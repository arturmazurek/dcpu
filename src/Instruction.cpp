//
//  Instruction.cpp
//  dcpu
//
//  Created by Artur Mazurek on 21.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Instruction.h"

#include <cassert>

Instruction::Instruction() : raw{0, 0} {

}

Instruction::Instruction(Opcode op, uint8_t b, uint8_t a) : Instruction{} {
    assert(a <= 0b111111);
    assert(b <= 0b11111);
    assert(op <= 0b11111);
    
    raw[1] = op;
    raw[1] |= (b << 5);
    
    raw[0] = b >> 3;
    raw[0] |= a << 2;
}

Instruction::Instruction(Opcode op, uint8_t a) : Instruction{} {
    assert(a <= 0b111111);
    assert(op <= 0b11111);
    
    raw[1] = 0;
    raw[1] |= (op << 5);
    
    raw[0] = op >> 3;
    raw[0] |= a << 2;
}
