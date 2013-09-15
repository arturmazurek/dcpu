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

#include "ASTVisitor.h"
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
    
    REQUIRE_TRUE(ast->label != nullptr, "Is label properly filled");
    CHECK_EQUAL(ast->label->identifier, "label", "Is label retrieved properly");
    
    CHECK_TRUE(ast->op == nullptr, "Is op correctly empty");
    CHECK_TRUE(ast->a == nullptr, "Is a correctly empty");
    CHECK_TRUE(ast->b == nullptr, "Is b correctly empty");
},

TEST {
    meta.name = "Just operation";
    
    std::stringstream s{"  op   "};
    Lexer l{s};
    Parser p;
    
    auto ast = p.parseCommand(l);
    
    REQUIRE_TRUE(ast->op != nullptr, "Is operation properly filled");
    CHECK_EQUAL(ast->op->identifier, "op", "Is label retrieved properly");
    
    CHECK_TRUE(ast->label == nullptr, "Is label correctly empty");
    CHECK_TRUE(ast->a == nullptr, "Is a correctly empty");
    CHECK_TRUE(ast->b == nullptr, "Is b correctly empty");
},

TEST {
    meta.name = "Base parsing";
    
    std::stringstream s{"aaaa: dddd"};
    Lexer l{s};
    Parser p;
    
    auto ast = p.parseCommand(l);
    
    REQUIRE_TRUE(ast->label != nullptr, "Is label properly found");
    CHECK_EQUAL(ast->label->identifier, "aaaa", "Is label retrieved properly");
    
    REQUIRE_TRUE(ast->op != nullptr, "Is operation properly found");
    CHECK_EQUAL(ast->op->identifier, "dddd", "Is operation retrieved properly");
    
    CHECK_TRUE(ast->a == nullptr, "Is operand a correctly empty");
    CHECK_TRUE(ast->b == nullptr, "Is operand b correctly empty");
},

TEST {
    meta.name = "No comma";
    
    std::stringstream s{"op a b"};
    Lexer l{s};
    Parser p;
    
    bool caught = false;
    try {
        auto ast = p.parseCommand(l);
    } catch(const ParserException& e) {
        caught = true;
    }
    
    CHECK_TRUE(caught, "An exception must be thrown when there's no comma");
},

TEST {
    meta.name = "Full line parsing";
    
    std::stringstream s{
        "label1: op1 a, [b] \n"\
        "label2: op2 [c],d"
    };
    
    Lexer l{s};
    Parser p;
    
    auto ast = p.parseCommand(l);
    
    REQUIRE_TRUE(ast->label != nullptr, "Is label properly found");
    CHECK_EQUAL(ast->label->identifier, "label1", "Is label retrieved properly");
    
    REQUIRE_TRUE(ast->op != nullptr, "Is operation properly found");
    CHECK_EQUAL(ast->op->identifier, "op1", "Is operation retrieved properly");
    
    REQUIRE_TRUE(ast->a != nullptr, "Is operator a properly found");
    CHECK_TRUE(!(ast->a->addressing), "Is operator a non-addressing");

    REQUIRE_TRUE(ast->b != nullptr, "Is operator b properly found");
    CHECK_TRUE(ast->b->addressing, "Is operator b addressing");
    
    ast = p.parseCommand(l);
    
    REQUIRE_TRUE(ast->label != nullptr, "Is label properly found 2");
    CHECK_EQUAL(ast->label->identifier, "label2", "Is label retrieved properly 2");
    
    REQUIRE_TRUE(ast->op != nullptr, "Is operation properly found 2");
    CHECK_EQUAL(ast->op->identifier, "op2", "Is operation retrieved properly 2");
    
    REQUIRE_TRUE(ast->a != nullptr, "Is operator c properly found");
    CHECK_TRUE(ast->a->addressing, "Is operator c non-addressing");
    
    REQUIRE_TRUE(ast->b != nullptr, "Is operator d properly found");
    CHECK_TRUE(!(ast->b->addressing), "Is operator d non-addressing");
},

TEST {
    meta.name = "Binary ops";
    
    std::stringstream s{
        "op a+1,b"
    };
    
    Lexer l{s};
    Parser p;
    
    auto ast = p.parseCommand(l);
    
    CHECK_TRUE(ast->label == nullptr, "Is there no label");
    
    REQUIRE_TRUE(ast->op != nullptr, "Is operation parsed");
    CHECK_EQUAL(ast->op->identifier, "op", "Is operation properly retrieved");
    
    REQUIRE_TRUE(ast->a != nullptr, "Is operand a retrieved");
    
    REQUIRE_TRUE(ast->b != nullptr, "Is operand b retrieved");
},

TESTS_END

#endif
