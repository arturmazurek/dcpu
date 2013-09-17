//
//  Constants.cpp
//  dcpu
//
//  Created by Artur Mazurek on 16.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Constants.h"

namespace Constants {
    const std::map<std::string, Registers::Code> REGISTER_NAMES {
        { "a", Registers::REG_A },
        { "b", Registers::REG_B },
        { "c", Registers::REG_C },
        { "x", Registers::REG_X },
        { "y", Registers::REG_Y },
        { "z", Registers::REG_Z },
        { "i", Registers::REG_I },
        { "j", Registers::REG_J },
        { "sp", Registers::REG_SP },
        { "pc", Registers::REG_PC },
        { "ex", Registers::REG_EX },
    };
    
#define OPCODES_INL
#define OPCODE(op, val) { #op, OP_##op }
    
    const std::map<std::string, Opcode> OPCODES_NAMES {
        #include "Opcodes.inl"
    };
    
#undef OPCODES_INL
#undef OPCODE
}