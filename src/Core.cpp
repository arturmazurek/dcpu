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

Core::Core() : m_decoded{0, 0, 0, 0} {
    
}

void Core::resetState() {
    m_memory.fill(0);
    
    memset(&m_registers, 0, sizeof(Registers));
    m_registers.SP = STACK_START;
    
    memset(&m_current, 0, sizeof(Instruction));
}

void Core::setMemory(uint16_t* m, unsigned size, unsigned startingAt) {
    assert(size + startingAt < MEMORY_SIZE);
    
    std::copy(m, m + size, begin(m_memory));
}

void Core::setInstructions(Instruction* m, unsigned size, unsigned startingAt) {
    setMemory(reinterpret_cast<uint16_t*>(m), size, startingAt);
}

void Core::doCycle() {
    if(m_decoded.costLeft >= 1) {
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
}

void Core::printRegisters() const {
    using namespace std;
    
    auto flags = cout.flags(ios::hex);
    auto fill = cout.fill('0');

    cout << " A: " << setw(4) << m_registers.A << "  B: " << setw(4) << m_registers.B
        << "  C: " << setw(4) << m_registers.C << "  D: " << setw(4) << m_registers.D << endl;
    
    cout << " X: " << setw(4) << m_registers.X << "  Y: " << setw(4) << m_registers.Y << "  Z: " << setw(4) << m_registers.Z
        << "  I: " << setw(4) << m_registers.I << "  J: " << setw(4) << m_registers.J << endl;
    
    cout << "PC: " << setw(4) << m_registers.PC << " SP: " << setw(4) << m_registers.SP << endl;
    cout << "EX: " << setw(4) << m_registers.EX << " IA: " << setw(4) << m_registers.IA << endl;
    
    cout.flags(flags);
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
    m_decoded.element1 = element1(m_current);
    m_decoded.element2 = element2(m_current);
    m_decoded.element3 = element3(m_current);
    
    m_decoded.costLeft = 1;
}

void Core::execute() {
    if(m_decoded.element3 != 0) {
        executeNormal();
    } else {
        executeSpecial();
    }
}

void Core::executeNormal() {
    switch (m_decoded.element3) {
        case OP_SET: {
            auto fill = std::cout.fill('0');
            std::cout << "SET " << std::hex << std::showbase << std::internal << std::setw(4) << (int)m_decoded.element2 << ", "
                        << std::hex << std::showbase << std::internal << std::setw(4) << (int)m_decoded.element1 << std::endl;
            std::cout.fill(fill);
            break;
        }
            
        default:
            std::cout << "Unhandled opcode: " << std::hex << m_decoded.element3 << std::endl;
            break;
    }
}

void Core::executeSpecial() {
    
}