//
//  LexerTests.h
//  dcpu
//
//  Created by Artur Mazurek on 12/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_LexerTests_h
#define dcpu_LexerTests_h

#include <sstream>

#include "Lexer.h"
#include "TestsUtil.h"

TESTS_START(LexerTests)

TEST {
    meta.name = "Lexing identifiers";
    
    std::stringstream s{"a b c d"};
    Lexer l{s};
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_IDENTIFIER, "Are identifiers properly recognised");
    CHECK_EQUAL(l.identifier(), "a", "Are identifiers properly returned");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_IDENTIFIER, "Are identifiers properly recognised 2");
    CHECK_EQUAL(l.identifier(), "b", "Are identifiers properly returned 2");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_IDENTIFIER, "Are identifiers properly recognised 3");
    CHECK_EQUAL(l.identifier(), "c", "Are identifiers properly returned 3");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_IDENTIFIER, "Are identifiers properly recognised 4");
    CHECK_EQUAL(l.identifier(), "d", "Are identifiers properly returned 4");
},

TEST {
    meta.name = "Lexing numbers";
    
    std::stringstream s{"1 23 456 7890  "};
    Lexer l{s};
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_NUMBER, "Are numbers properly recognised");
    CHECK_EQUAL(l.number(), 1, "Are numbers properly returned");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_NUMBER, "Are numbers properly recognised 2");
    CHECK_EQUAL(l.number(), 23, "Are numbers properly returned 2");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_NUMBER, "Are numbers properly recognised 3");
    CHECK_EQUAL(l.number(), 456, "Are numbers properly returned 3");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_NUMBER, "Are numbers properly recognised 4");
    CHECK_EQUAL(l.number(), 7890, "Are numbers properly returned 4");
},

TEST {
    meta.name = "Mixed lexing";
    
    std::stringstream s{"123 dsa, , &* dw1 repeat repeat1"};
    Lexer l{s};
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_NUMBER, "Are numbers properly recognised");
    CHECK_EQUAL(l.number(), 123, "Are proper number values returned");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_IDENTIFIER, "Are identifiers properly recognised");
    CHECK_EQUAL(l.identifier(), "dsa", "Are proper identifiers returned");
    
    CHECK_EQUAL(l.nextToken(), ',', "Are commas properly recognised");
    
    CHECK_EQUAL(l.nextToken(), ',', "Are commas properly recognised 2");

    CHECK_EQUAL(l.nextToken(), '&', "Are special characters properly recognised & returned");

    CHECK_EQUAL(l.nextToken(), '*', "Are special characters properly recognised & returned 2");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_IDENTIFIER, "Are identifiers properly recognised 2");
    CHECK_EQUAL(l.identifier(), "dw1", "Are proper identifiers returned");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_REPEAT, "Are keywords properly recognised & returned 3");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_IDENTIFIER, "Are identifiers properly recognised");
    CHECK_EQUAL(l.identifier(), "repeat1", "Are proper identifiers returned");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_EOF, "Is end of input recognised and returned correctly");
},

TEST {
    meta.name = "Lexer exception";
    
    std::stringstream s{"12 03 a"};
    Lexer l{s};
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_NUMBER, "Are numbers properly recognised");
    CHECK_EQUAL(l.number(), 12, "Are numbers properly returned");
    
    bool caught = false;
    try {
        l.nextToken();
    } catch (LexerException& e) {
        caught = true;
    }
    
    CHECK_TRUE(caught, "Is a lexer exception thrown and caught");
},

TEST {
    meta.name = "Lexer line breaks";
    
    std::stringstream s{"12 \n as \n"};
    Lexer l{s};
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_NUMBER, "Are numbers properly recognised");
    CHECK_EQUAL(l.number(), 12, "Are numbers properly returned");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_ENDLINE, "Are endlines properly recognised");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_IDENTIFIER, "Are identifiers properly recognised");
    CHECK_EQUAL(l.identifier(), "as", "Are identifiers properly returned");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_ENDLINE, "Are endlines properly recognised");
    
    CHECK_EQUAL(l.nextToken(), Lexer::TOK_EOF, "Is input end properly recognised");
},

TESTS_END

#endif
