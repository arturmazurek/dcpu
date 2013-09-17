//
//  Opcodes.h
//  dcpu
//
//  Created by Artur Mazurek on 17/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_Opcodes_h
#define dcpu_Opcodes_h

#include <map>

#define OPCODES_INL
#define OPCODE(op, value) OP_##op = value

enum Opcode {
    #include "Opcodes.inl"
};

constexpr static inline bool isConditional(Opcode op) {
    return op >= OP_IFB && op <= OP_IFU;
}

#undef OPCODE
#undef OPCODES_INL

#endif
