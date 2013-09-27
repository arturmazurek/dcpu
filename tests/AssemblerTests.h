//
//  AssemblerTests.h
//  dcpu
//
//  Created by Artur on 25/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_AssemblerTests_h
#define dcpu_AssemblerTests_h

#include <sstream>

#include "TestsUtil.h"

#include "ASMUtils.h"
#include "Assembler.h"
#include "Lexer.h"

TESTS_START(AssemblerTests)

TEST {
    meta.name = "Basic assembler";
    
    std::stringstream s{"set a, 1"};
    
    Assembler assembler;
    assembler.setLexer(std::make_unique<Lexer>(s));
    assembler.assemble();
    
    const auto& binary = assembler.binary();
    REQUIRE_EQUAL(binary.size(), 1, "Is binary assembled");
    
    Instruction i = { OP_SET, 0x00, 0x22 }; /// SET A, 1
    uint16_t val = *reinterpret_cast<uint16_t*>(i.raw);
    CHECK_EQUAL(binary[0], val, "Is assembled correctly");
    
    core.setMemory(binary);
    core.cycle();
    
    CHECK_EQUAL(core.registers().A, 1, "Is register updated correctly");
},

TEST {
    meta.name = "More complicated asembler";
    
    std::stringstream s{
        "set a, 1  \n\
         add a, 124"
    };
    
    Assembler assembler;
    assembler.setLexer(std::make_unique<Lexer>(s));
    assembler.assemble();
    
    core.setMemory(assembler.binary());
    core.cycle(1);
    CHECK_EQUAL(core.registers().A, 1, "Is A set correctly");

    core.cycle(3);
    CHECK_EQUAL(core.registers().A, 125, "Is addition correct");
},

TESTS_END

#endif
