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

struct Instruction {
    unsigned int cycles;
    
    union Value {
        uint16_t raw;
        
        struct Decoded {
            uint16_t a : 6;
            uint16_t b : 5;
            uint16_t o : 5;
        } decoded;
    } value;
};

#endif
