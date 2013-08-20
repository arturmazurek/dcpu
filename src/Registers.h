//
//  Registers.h
//  dcpu
//
//  Created by Artur Mazurek on 20.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_Registers_h
#define dcpu_Registers_h

#include <cstdint>

struct Registers {
    uint16_t A, B, C, D, X, Y, Z, I, J;
    uint16_t PC;
    uint16_t SP;
    uint16_t EX;
    uint16_t IA;
};

#endif
