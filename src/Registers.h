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
