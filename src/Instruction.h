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
    uint8_t raw[2];
    
    Instruction(Opcode op, uint8_t b, uint8_t a);
    Instruction(Opcode op, uint8_t a);
    Instruction();
};

static inline uint8_t element3(const Instruction& instr) {
    return instr.raw[1] & 0b00011111;
}

static inline uint8_t element2(const Instruction& instr) {
    uint8_t result = (instr.raw[1] & 0b11100000) >> 5;
    result |= (instr.raw[0] & 0b00000011) << 3;
    
    return result;
}

static inline uint8_t element1(const Instruction& instr) {
    return (instr.raw[0] & 0b11111100) >> 2;
}

#endif
