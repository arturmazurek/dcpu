//
//  Instruction.h
//  dcpu
//
//  Created by Artur Mazurek on 20.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_Instruction_h
#define dcpu_Instruction_h

#include <cstdint>

#include "Opcodes.h"

struct Instruction {
    char raw[2];
    
    Instruction(Opcode op, uint8_t b, uint8_t a);
    Instruction(Opcode op, uint8_t a);
    Instruction();
};

#endif
