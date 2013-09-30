//
//  Constants.cpp
//  dcpu
//
//  Created by Artur Mazurek on 16.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Constants.h"

#include <algorithm>
#include <cctype>

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
    
    // Used to make lower case operand names (assembler uses lowercase)
    class ToLower {
    public:
        explicit ToLower(const std::string& s) : m_string{s} {
            std::transform(m_string.begin(), m_string.end(), m_string.begin(), std::tolower);
        }
        
        operator std::string() {
            return m_string;
        }
        
    private:
        std::string m_string;
    };
    
#define OPCODES_INL
#define OPCODE(op, val) { ToLower{#op}, OP_##op }
    
    const std::map<std::string, Opcode> OPCODES_NAMES {
        #include "Opcodes.inl"
    };
    
    const std::map<std::string, Opcode> SPECIAL_OPCODES_NAMES {
        #include "SpecialOpcodes.inl"
    };
    
#undef OPCODES_INL
#undef OPCODE
}