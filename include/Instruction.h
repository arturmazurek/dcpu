//
//  Instruction.h
//  dcpu
//
//  Created by Artur Mazurek on 20.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_Instruction_h
#define dcpu_Instruction_h

#include <cassert>
#include <cstdint>

#include <bitset>
#include <ostream>

#include "Opcodes.h"

struct Instruction final {
    uint8_t raw[2];
    
    Instruction(uint16_t data) {
        raw[0] = data;
        raw[1] = data >> 8;
    }
    
    Instruction(Opcode op, uint8_t b, uint8_t a) {
        assert(a <= 0b111111);
        assert(b <= 0b11111);
        assert(op <= 0b11111);
        
        raw[1] = op;
        raw[1] |= (b << 5);
        
        raw[0] = b >> 3;
        raw[0] |= a << 2;
    }
    
    Instruction(Opcode op, uint8_t a) {
        assert(a <= 0b111111);
        assert(op <= 0b11111);
        
        raw[1] = 0;
        raw[1] |= (op << 5);
        
        raw[0] = op >> 3;
        raw[0] |= a << 2;
    }
    
    Instruction() : raw{0, 0} {
        
    }
};

static inline std::ostream& operator<<(std::ostream& os, const Instruction& instr) {
    os << std::bitset<8>(instr.raw[0]) << std::bitset<8>(instr.raw[1]);
    return os;
}

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
