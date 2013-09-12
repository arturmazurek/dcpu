//
//  ParserTests.h
//  dcpu
//
//  Created by Artur Mazurek on 12/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_ParserTests_h
#define dcpu_ParserTests_h

#include <sstream>

#include "Lexer.h"
#include "Parser.h"
#include "TestsUtil.h"

TESTS_START(ParserTests)

TEST {
    meta.name = "Just label";
    
    std::stringstream s{"label:   \n\n "};
    Lexer l{s};
    Parser p;
    
    auto ast = p.parseCommand(l);
    
    CHECK_TRUE(ast->label != nullptr, "Is label properly filled");
    CHECK_EQUAL(ast->label->ident(), "label", "Is label retrieved properly");
    
    CHECK_TRUE(ast->op == nullptr, "Is op correctly empty");
    CHECK_TRUE(ast->op == nullptr, "Is op correctly empty");
    CHECK_TRUE(ast->op == nullptr, "Is op correctly empty");
},

TEST {
    meta.name = "Base parsing";
    
    std::stringstream s{"aaaa: dddd"};
    Lexer l{s};
    Parser p;
    
    auto ast = p.parseCommand(l);
    
    CHECK_TRUE(ast->label != nullptr, "Is label properly found");
    CHECK_EQUAL(ast->label->ident(), "aaaa", "Is label retrieved properly");
    
    CHECK_TRUE(ast->op != nullptr, "Is operation properly found");
    CHECK_EQUAL(ast->op->ident(), "dddd", "Is operation retrieved properly");
    
    CHECK_TRUE(ast->a == nullptr, "Is operand a correctly empty");
    CHECK_TRUE(ast->b == nullptr, "Is operand b correctly empty");
},

TESTS_END

#endif
