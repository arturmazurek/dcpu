//
//  Core.cpp
//  dcpu
//
//  Created by Artur Mazurek on 20.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Core.h"

#include <cassert>
#include <cstdlib>

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "CostCalculator.h"
#include "DCPUException.h"
#include "Hardware.h"

const std::chrono::milliseconds Core::DEFAULT_PERIOD{1};
static constexpr std::chrono::microseconds MINIMUM_PERIOD{10};

Core::Core() : m_decoded{0}, m_skipping{false}, m_queueInterrupts{false}, m_period{DEFAULT_PERIOD}, m_minimumPeriod{MINIMUM_PERIOD}, m_running{false}, m_shouldRun{false} {
    
}

Core::~Core() {
    stop();
    if(m_runThread.joinable()) {
        m_runThread.join();
    }
}

void Core::resetState() {
    m_memory.fill(0);
    memset(&m_registers, 0, sizeof(Registers));
    memset(&m_current, 0, sizeof(Instruction));
    m_skipping = false;
}

const Registers& Core::registers() const {
    return const_cast<Core*>(this)->registers();
}

Registers& Core::registers() {
    return m_registers;
}

uint16_t Core::memory(uint16_t at) const {
    return m_memory[at];
}

void Core::setMemory(uint16_t* m, unsigned size, unsigned startingAt) {
    assert(size + startingAt < MEMORY_SIZE);
    
    std::copy(m, m + size, begin(m_memory));
}

void Core::setInstructions(Instruction* m, unsigned size, unsigned startingAt) {
    setMemory(reinterpret_cast<uint16_t*>(m), size, startingAt);
}

void Core::attachHardware(std::shared_ptr<Hardware> hardware) {
    std::lock_guard<decltype(m_hardwareMutex)> lock{m_hardwareMutex};
    
    assert(m_attachedHardware.size() <= std::numeric_limits<uint16_t>::max() && "Too much hardware added");
    
    auto location = std::find(m_attachedHardware.begin(), m_attachedHardware.end(), hardware);
    assert(location == m_attachedHardware.end() && "Cannot attach hardware twice");

    m_attachedHardware.emplace_back(hardware);
    hardware->attachedTo(this);
}

bool Core::hasHardware(std::shared_ptr<Hardware> hardware) const {
    std::lock_guard<decltype(m_hardwareMutex)> lock{m_hardwareMutex};
    
    return std::find(m_attachedHardware.begin(), m_attachedHardware.end(), hardware) != m_attachedHardware.end();
}

void Core::detachHardware(std::shared_ptr<Hardware> hardware) {
    std::lock_guard<decltype(m_hardwareMutex)> lock{m_hardwareMutex};
    
    auto location = std::find(m_attachedHardware.begin(), m_attachedHardware.end(), hardware);
    assert(location != m_attachedHardware.end() && "Cannot detach unattached hardware");
    
    m_attachedHardware.erase(location);
}

void Core::run(std::chrono::microseconds microseconds) {
    m_running = true;
    m_shouldRun = true;
    m_period = microseconds;
    
    auto runFn = [&]() {
        while (m_shouldRun) {
            auto start = std::chrono::high_resolution_clock::now();
            this->doCycle();
            auto end = std::chrono::high_resolution_clock::now();
            
            // assumes monotonic clock
            auto duration = end - start;
            auto sleepTime = m_period - duration;
            sleepTime = sleepTime < m_minimumPeriod ? m_minimumPeriod : sleepTime;
            
            std::this_thread::sleep_for(sleepTime);
        }
        m_running = false;
    };
    
    m_runThread = std::thread{runFn};
}

void Core::stop() {
    m_shouldRun = false;
}

void Core::cycle(unsigned cycles) {
    assert(!m_running);
    while(cycles != 0) {
        doCycle();
        --cycles;
    }
}

void Core::doCycle() {
    if(m_decoded.costLeft == 0) {            
        fetch();
        decode();
        assert(m_decoded.costLeft != 0 && "No instruction can be worth 0");
        
        if (m_skipping) {
            m_decoded.costLeft = 1;
        }
    }
    
    --m_decoded.costLeft;
    if(m_decoded.costLeft == 0) {
        if(!m_skipping) {
            execute();
        } else if(!isConditional(m_decoded.opcode)) {
            m_skipping = false;
        }
        
        if(!m_skipping) {
            handleInterrupt();
        }
    }
}

void Core::printRegisters() const {
    using namespace std;
    
    auto flags = cout.flags(ios::hex);
    auto fill = cout.fill('0');

    cout << " A: " << setw(4) << m_registers.A << "  B: " << setw(4) << m_registers.B
        << "  C: " << setw(4) << m_registers.C << endl;
    
    cout << " X: " << setw(4) << m_registers.X << "  Y: " << setw(4) << m_registers.Y << "  Z: " << setw(4) << m_registers.Z
        << "  I: " << setw(4) << m_registers.I << "  J: " << setw(4) << m_registers.J << endl;
    
    cout << "PC: " << setw(4) << m_registers.PC << " SP: " << setw(4) << m_registers.SP << endl;
    cout << "EX: " << setw(4) << m_registers.EX << " IA: " << setw(4) << m_registers.IA << endl;
    
    cout.flags(flags);
    cout.fill(fill);
}

void Core::printMemory(uint16_t start, uint16_t end) const {
    using namespace std;
    auto fill = cout.fill('0');
    
    while(start < end) {
        uint16_t val = m_memory[start];
        char *temp = reinterpret_cast<char*>(&val);
        
        cout << bitset<8>(temp[0]) << bitset<8>(temp[1]) << " 0x" << hex << setw(4) << m_memory[start] << endl;
        ++start;
    }
    
    cout.fill(fill);
}

bool Core::interruptsEnabled() const {
    std::lock_guard<decltype(m_interruptsMutex)> lock{m_interruptsMutex};
    return m_registers.IA != 0;
}

bool Core::queueInterrupts() const {
    std::lock_guard<decltype(m_interruptsMutex)> lock{m_interruptsMutex};
    return m_queueInterrupts;
}

void Core::setQueueInterrupts(bool queueInterrupts) {
    std::lock_guard<decltype(m_interruptsMutex)> lock{m_interruptsMutex};
    m_queueInterrupts = queueInterrupts;
}

void Core::handleInterrupt() {
    std::lock_guard<decltype(m_interruptsMutex)> lock(m_interruptsMutex);
    
    if(!interruptsEnabled()) {
        return;
    }
    
    if(queueInterrupts()) {
        return;
    }
    
    if(m_interruptsQueue.empty()) {
        return;
    }
    if(m_interruptsQueue.size() > MAX_INTERRUPTS) {
        throw TooManyInterruptsException{};
    }
    
    setQueueInterrupts(true);
    
    m_memory[--m_registers.SP] = m_registers.PC;
    m_memory[--m_registers.SP] = m_registers.A;
    
    m_registers.A = m_interruptsQueue.front();
    m_interruptsQueue.pop();
    m_registers.PC = m_registers.IA;
}

void Core::fetch() {
    uint16_t instr = m_memory[m_registers.PC];
    ++m_registers.PC;
    
    memcpy(m_current.raw, &instr, sizeof(instr));
}

void Core::decode() {
    m_decoded.opcode = static_cast<Opcode>(element3(m_current));
    
    if(m_decoded.opcode == OP_NONE) {
        // special opcode
        m_decoded.opcode = static_cast<Opcode>(element2(m_current));
        m_decoded.a = element1(m_current);
        m_decoded.costLeft = CostCalculator::getSpecialCost(m_decoded.opcode, m_decoded.a);
        
        m_decoded.source = checkOperand(m_decoded.a, true);
        
        m_decoded.special = true;
    } else {
        m_decoded.b = element2(m_current);
        m_decoded.a = element1(m_current);
        m_decoded.costLeft = CostCalculator::getNormalCost(m_decoded.opcode, m_decoded.b, m_decoded.a);
        
        m_decoded.source = checkOperand(m_decoded.a, true);
        m_decoded.target = checkOperand(m_decoded.b, false);
        
        m_decoded.special = false;
    }
}

uint16_t* Core::checkOperand(uint16_t& operand, bool inA) {
    if(operand <= 0x07) {
        // register (A, B, C, X, Y, Z, I or J, in that order)
        return &m_registers.value(static_cast<RegisterCode>(operand));
    } else if(operand <= 0x0f) {
        // [register]
        return &m_memory[m_registers.value(static_cast<RegisterCode>(operand - 0x08))];
    } else if(operand <= 0x17) {
        // [register + next word]
        uint16_t nextWord = m_memory[m_registers.PC++];
        return &m_memory[nextWord + m_registers.value(static_cast<RegisterCode>(operand - 0x10))];
    } else if(operand == 0x18) {
        // (PUSH / [--SP]) if in b, or (POP / [SP++]) if in a
        if(inA) {
            return &m_memory[m_registers.SP++];
        } else {
            return &m_memory[--m_registers.SP];
        }
    } else if(operand == 0x19) {
        // [SP] / PEEK
        return &m_memory[m_registers.SP];
    } else if(operand == 0x1a) {
        // [SP + next word] / PICK n
        uint16_t nextWord = m_memory[m_registers.PC++];
        return &m_memory[m_registers.SP + nextWord];
    } else if(operand == 0x1b) {
        // SP
        return &m_registers.SP;
    } else if(operand == 0x1c) {
        // PC
        return &m_registers.PC;
    } else if(operand == 0x1d) {
        // EX
        return &m_registers.EX;
    } else if(operand == 0x1e) {
        // [next word]
        return &m_memory[m_memory[m_registers.PC++]];
    } else if(operand == 0x1f) {
        // next word (literal)
        return &m_memory[m_registers.PC++];
    } else if(operand == 0x20 ) {
        // literal value 0xffff (only for a)
        assert(inA);
        operand = 0xffff;
        return &operand;
    } else if(operand <= 0x3f) {
        // literal value 0x0-0x1e (0..30) (literal) (only for a)
        assert(inA);
        operand = operand - 0x21;
        return &operand;
    } else {
        assert(!"Unknown parameter");
    }
}

void Core::execute() {
    if(!m_decoded.special) {
        executeNormal();
    } else {
        executeSpecial();
    }
}

void Core::executeNormal() {
    switch (m_decoded.opcode) {
        case OP_SET:
            *m_decoded.target = *m_decoded.source;
            break;
            
        case OP_ADD: {
            uint32_t temp = *m_decoded.target;
            temp += *m_decoded.source;
            *m_decoded.target = temp;
            
            if(temp > std::numeric_limits<uint16_t>::max()) {
                m_registers.EX = 0x0001;
            } else {
                m_registers.EX = 0;
            }
            break;
        }
            
        case OP_SUB: {
            uint16_t source = *m_decoded.source;
            uint16_t target = *m_decoded.target;
            
            if(target < source) {
                m_registers.EX = 0xffff;
            } else {
                m_registers.EX = 0;
            }
            
            *m_decoded.target = target - source;
            break;
        }
            
        case OP_MUL: {
            uint32_t temp = *m_decoded.target;
            temp *= *m_decoded.source;
            
            *m_decoded.target = temp;
            m_registers.EX = temp >> 16;
            break;
        }
            
        case OP_MLI: {
            int32_t temp = *m_decoded.target;
            temp *= *m_decoded.source;
            
            *m_decoded.target = static_cast<int16_t>(temp);
            m_registers.EX = static_cast<int16_t>(temp) >> 16;
            break;
        }
            
        case OP_DIV: {
            if(*m_decoded.source == 0) {
                *m_decoded.target = 0;
                m_registers.EX = 0;
                break;
            }
            
            uint32_t temp = *m_decoded.target;
            
            *m_decoded.target = temp / *m_decoded.source;
            m_registers.EX = (temp << 16) / *m_decoded.source;     
            break;
        }
            
        case OP_DVI: {
            if(*m_decoded.source == 0) {
                *m_decoded.target = 0;
                m_registers.EX = 0;
                break;
            }
            
            int32_t temp = static_cast<int16_t>(*m_decoded.target);
            
            *m_decoded.target = temp / static_cast<int32_t>(*m_decoded.source);
            m_registers.EX = (temp << 16) / static_cast<int32_t>(*m_decoded.source);
            break;
        }
            
        case OP_MOD: {
            if(*m_decoded.source == 0) {
                *m_decoded.target = 0;
                break;
            }
            
            *m_decoded.target %= *m_decoded.source;
            break;
        }
            
        case OP_MDI: {
            if(*m_decoded.source == 0) {
                *m_decoded.target = 0;
                break;
            }
            
            int16_t temp = *m_decoded.target;
            temp %= static_cast<int16_t>(*m_decoded.source);
            *m_decoded.target = temp;
            break;
        }
            
        case OP_AND: {
            *m_decoded.target &= *m_decoded.source;
            break;
        }
            
        case OP_BOR: {
            *m_decoded.target |= *m_decoded.source;
            break;
        }

        case OP_XOR: {
            *m_decoded.target ^= *m_decoded.source;
            break;
        }
            
        case OP_SHR: {
            uint32_t temp = static_cast<uint32_t>(*m_decoded.target) << 16;
            temp >>= *m_decoded.source;
            m_registers.EX = temp;
            
            *m_decoded.target >>= *m_decoded.source;
            break;
        }
            
        case OP_ASR: {
            int32_t temp = static_cast<int32_t>(*m_decoded.target) << 16;
            temp >>= *m_decoded.source;
            m_registers.EX = temp;
            
            int16_t tempVal = *m_decoded.target;
            tempVal >>= *m_decoded.source;
            *m_decoded.target = tempVal;
            break;
        }
            
        case OP_SHL: {
            int32_t temp = static_cast<int32_t>(*m_decoded.target) << *m_decoded.source;
            temp >>= 16;
            m_registers.EX = temp;
            
            *m_decoded.target <<= *m_decoded.source;
            break;
        }
            
        case OP_IFB: {
            bool success = (*m_decoded.source & *m_decoded.target) != 0;
            if(!success) {
                m_skipping = true;
            }
            
            break;
        }
            
        case OP_IFC: {
            bool success = (*m_decoded.source & *m_decoded.target) == 0;
            if(!success) {
                m_skipping = true;
            }
            
            break;
        }
            
        case OP_IFE: {
            bool success = *m_decoded.source == *m_decoded.target;
            if(!success) {
                m_skipping = true;
            }
            
            break;
        }
            
        case OP_IFN: {
            bool success = *m_decoded.source != *m_decoded.target;
            if(!success) {
                m_skipping = true;
            }
            
            break;
        }
            
        case OP_IFG: {
            bool success = *m_decoded.target > *m_decoded.source;
            if(!success) {
                m_skipping = true;
            }
            
            break;
        }
            
        case OP_IFA: {
            bool success = static_cast<int16_t>(*m_decoded.target) > static_cast<int16_t>(*m_decoded.source);
            if(!success) {
                m_skipping = true;
            }
            
            break;
        }
            
        case OP_IFL: {
            bool success = *m_decoded.target < *m_decoded.source;
            if(!success) {
                m_skipping = true;
            }
            
            break;
        }
            
        case OP_IFU: {
            bool success = static_cast<int16_t>(*m_decoded.target) < static_cast<int16_t>(*m_decoded.source);
            if(!success) {
                m_skipping = true;
            }
            
            break;
        }
            
        case OP_ADX: {
            uint32_t temp = *m_decoded.source + *m_decoded.target + m_registers.EX;
            if(temp > std::numeric_limits<uint16_t>::max()) {
                m_registers.EX = 1;
            } else {
                m_registers.EX = 0;
            }
            
            *m_decoded.target = temp;
            break;
        }
            
        case OP_SBX: {
            uint32_t temp = *m_decoded.target - *m_decoded.source + m_registers.EX;
            if(*m_decoded.target + m_registers.EX < *m_decoded.source) {
                m_registers.EX = 0xffff;
            } else if (temp > std::numeric_limits<uint16_t>::max()) {
                m_registers.EX = 1;
            } else {
                m_registers.EX = 0;
            }
            
            *m_decoded.target = temp;
            break;
        }
            
        case OP_STI: {
            *m_decoded.target = *m_decoded.source;
            ++m_registers.I;
            ++m_registers.J;
            
            break;
        }
            
        case OP_STD: {
            *m_decoded.target = *m_decoded.source;
            --m_registers.I;
            --m_registers.J;
            
            break;
        }
            
        default:
            std::cout << "Unhandled opcode: " << std::hex << std::showbase << m_decoded.opcode << " from: " << m_current << std::endl;
            break;
    }
}

void Core::executeSpecial() {
    switch (m_decoded.opcode) {
        case OP_JSR:
            m_memory[--m_registers.SP] = m_registers.PC;
            m_registers.PC = *m_decoded.source;
            break;
            
        case OP_INT:
            receiveInterrupt(*m_decoded.source);
            break;
            
        case OP_IAG:
            *m_decoded.source = m_registers.IA;
            break;
            
        case OP_IAS:
            m_registers.IA = *m_decoded.source;
            break;
            
        case OP_RFI:
            setQueueInterrupts(false);
            m_registers.A = m_memory[m_registers.SP++];
            m_registers.PC = m_memory[m_registers.SP++];
            break;
            
        case OP_IAQ:
            setQueueInterrupts(*m_decoded.source == 0);
            break;
            
        case OP_HWN:
            m_hardwareMutex.lock();
            *m_decoded.source = m_attachedHardware.size();
            m_hardwareMutex.unlock();
            break;
            
        case OP_HWQ: {
            std::lock_guard<decltype(m_hardwareMutex)> lock{m_hardwareMutex};
            
            uint16_t index = *m_decoded.source;
            if(index >= m_attachedHardware.size()) {
                std::cout << "Malformed program - trying to access unexistent hardware" << std::endl;
                break;
            }
            
            m_registers.A = m_attachedHardware[index]->hardwareId();
            m_registers.B = m_attachedHardware[index]->hardwareId() >> 16;
            m_registers.C = m_attachedHardware[index]->version();
            m_registers.X = m_attachedHardware[index]->manufacturer();
            m_registers.Y = m_attachedHardware[index]->manufacturer() >> 16;
        } break;
            
        case OP_HWI: {
            std::lock_guard<decltype(m_hardwareMutex)> lock{m_hardwareMutex};
            
            uint16_t index = *m_decoded.source;
            if(index >= m_attachedHardware.size()) {
                std::cout << "Malformed program - trying to access unexistent hardware" << std::endl;
                break;
            }
            
            m_attachedHardware[index]->receiveInterrupt();
        } break;
            
        default:
            std::cout << "Unhandled special: " << std::hex << std::showbase << m_decoded.opcode << " from: " << m_current << std::endl;
            break;
    }
}

void Core::receiveInterrupt(uint16_t message) {
    std::lock_guard<decltype(m_interruptsMutex)> lock{m_interruptsMutex};
    m_interruptsQueue.emplace(message);
}