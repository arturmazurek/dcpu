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

#include "RegisterCode.h"

struct Registers {

    uint16_t A, B, C, X, Y, Z, I, J;
    uint16_t PC;
    uint16_t SP;
    uint16_t EX;
    uint16_t IA;
    
    uint16_t& value(RegisterCode reg) {
        switch(reg) {
            case REG_A: return A;
            case REG_B: return B;
            case REG_C: return C;
            case REG_X: return X;
            case REG_Y: return Y;
            case REG_Z: return Z;
            case REG_I: return I;
            case REG_J: return J;
            case REG_SP: return SP;
            case REG_PC: return PC;
            case REG_EX: return EX;
                
            default:
                assert(!"Unknown value of register to lookup");
        }
    }
};

#endif
