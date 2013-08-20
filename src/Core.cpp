//
//  Core.cpp
//  dcpu
//
//  Created by Artur Mazurek on 20.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Core.h"

Core::Core() {
    
}

void Core::resetState() {
    m_memory.fill(0);
    
    memset(&m_registers, 0, sizeof(Registers));
    m_registers.SP = STACK_START;
    
    memset(&m_current, 0, sizeof(Instruction));
}

void Core::doCycle() {
    
}