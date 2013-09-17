//
//  Constants.h
//  dcpu
//
//  Created by Artur Mazurek on 16.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_Constants_h
#define dcpu_Constants_h

#include <map>
#include <string>

#include "Opcodes.h"
#include "RegisterCode.h"

namespace Constants {
    extern const std::map<std::string, RegisterCode> REGISTER_NAMES;
    extern const std::map<std::string, Opcode> OPCODES_NAMES;
    
    static constexpr int ADDRESSING = 0x08;
    static constexpr int ADDRESSING_AND_NEXT_WORD = 0x10;
    static constexpr int PUSH_POP = 0x18;
    static constexpr int PEEK = 0x19;
    static constexpr int PICK = 0x1a;
    static constexpr int NEXT_WORD_ADDR = 0x1e; // [next word]
    static constexpr int NEXT_WORD = 0x1f; // next word (literal)
    static constexpr int LITERALS_MINUS_1 = 0x20; // -1
    static constexpr int LITERALS_30 = 0x3f; // 30
}

#endif
