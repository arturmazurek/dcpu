//
//  Constants.cpp
//  dcpu
//
//  Created by Artur Mazurek on 16.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Constants.h"

namespace Constants {
    const std::map<std::string, RegisterCode> REGISTER_NAMES {
        { "a", REG_A },
        { "b", REG_B },
        { "c", REG_C },
        { "x", REG_X },
        { "y", REG_Y },
        { "z", REG_Z },
        { "i", REG_I },
        { "j", REG_J },
        { "sp", REG_SP },
        { "pc", REG_PC },
        { "ex", REG_EX },
    };
    
#define OPCODES_INL
#define OPCODE(op, val) { #op, OP_##op }
    
    const std::map<std::string, Opcode> OPCODES_NAMES {
        #include "Opcodes.inl"
    };
    
#undef OPCODES_INL
#undef OPCODE
}