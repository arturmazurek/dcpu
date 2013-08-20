//
//  Core.cpp
//  dcpu
//
//  Created by Artur Mazurek on 20.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Core.h"

#include <iomanip>
#include <iostream>

Core::Core() {
    
}

void Core::resetState() {
    m_memory.fill(0);
    
    memset(&m_registers, 0, sizeof(Registers));
    m_registers.SP = STACK_START;
    
    memset(&m_current, 0, sizeof(Instruction));
}

void Core::doCycle() {
    if(m_current.cyclesLeft > 1) {
        --m_current.cyclesLeft;
        return;
    }
    
    if(interruptsEnabled()) {
        if(handleInterrupt()) {
            return;
        }
    }
    
    fetchAndDecode();
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

void Core::fetchAndDecode() {
    
}

void Core::execute() {
    
}