//
//  Registers.h
//  dcpu
//
//  Created by Artur Mazurek on 20.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_Registers_h
#define dcpu_Registers_h

#include <cassert>
#include <cstdint>

struct Registers {
    enum Code {
        REG_A = 0x00,
        REG_B = 0x01,
        REG_C = 0x02,
        REG_X = 0x03,
        REG_Y = 0x04,
        REG_Z = 0x05,
        REG_I = 0x06,
        REG_J = 0x07,
        REG_SP = 0x1b,
        REG_PC = 0x1c,
        REG_EX = 0x1d
    };
    
    uint16_t A, B, C, X, Y, Z, I, J;
    uint16_t PC;
    uint16_t SP;
    uint16_t EX;
    uint16_t IA;
    
    uint16_t& reg(int index) {
        assert(index <= 12 && "There are 12 registers only");
        char* start = reinterpret_cast<char*>(this);
        start += sizeof(uint16_t) * index;
        uint16_t* result = reinterpret_cast<uint16_t*>(start);
        
        return *result;
    }
};

#endif
