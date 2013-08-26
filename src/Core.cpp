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

Core::Core() : m_decoded{0} {
    
}

void Core::resetState() {
    m_memory.fill(0);
    memset(&m_registers, 0, sizeof(Registers));
    memset(&m_current, 0, sizeof(Instruction));
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

void Core::doCycle(unsigned cycles) {
    if(cycles == 0) {
        return;
    }
    
    while(cycles != 0) {
        if(m_decoded.costLeft > 1) {
            --m_decoded.costLeft;
            return;
        }
        
        if(interruptsEnabled()) {
            if(handleInterrupt()) {
                return;
            }
        }
        
        fetch();
        decode();
        execute();
        
        --cycles;
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
    return false;
}

bool Core::handleInterrupt() {
    return false;
}

void Core::fetch() {
    uint16_t instr = m_memory[m_registers.PC];
    ++m_registers.PC;
    
    memcpy(m_current.raw, &instr, sizeof(instr));
}

void Core::decode() {
    m_decoded.opcode = static_cast<Opcode>(element3(m_current));
    m_decoded.costLeft = 1;
    
    if(m_decoded.opcode == OP_NONE) {
        // special opcode
        m_decoded.opcode = static_cast<Opcode>(element2(m_current));
        m_decoded.a = element1(m_current);
        
        m_decoded.source = checkOperand(m_decoded.a, true);
    } else {
        m_decoded.b = element2(m_current);
        m_decoded.a = element1(m_current);
        
        m_decoded.source = checkOperand(m_decoded.a, true);
        m_decoded.target = checkOperand(m_decoded.b, false);
    }
}

uint16_t* Core::checkOperand(uint16_t& operand, bool inA) {
    if(operand <= 0x07) {
        // register (A, B, C, X, Y, Z, I or J, in that order)
        return &m_registers.reg(operand);
    } else if(operand <= 0x0f) {
        // [register]
        return &m_memory[m_registers.reg(operand - 0x08)];
    } else if(operand <= 0x17) {
        // [register + next word]
        uint16_t nextWord = m_memory[m_registers.PC++];
        return &m_memory[nextWord + m_registers.reg(operand - 0x10)];
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
            
        default:
            std::cout << "Unhandled opcode: " << std::hex << std::showbase << m_decoded.opcode << " from: " << m_current << std::endl;
            break;
    }
}

void Core::executeSpecial() {
    
}