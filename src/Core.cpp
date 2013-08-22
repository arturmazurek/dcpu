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
        return &m_memory[nextWord + m_registers.reg(operand - 0x16)];
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
            
        default:
            std::cout << "Unhandled opcode: " << std::hex << m_decoded.opcode << std::endl;
            break;
    }
}

void Core::executeSpecial() {
    
}