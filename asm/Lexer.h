//
//  Lexer.h
//  dcpu
//
//  Created by Artur Mazurek on 11/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __dcpu__Lexer__
#define __dcpu__Lexer__

#include <istream>
#include <map>
#include <string>

#include "LexerException.h"

// This takes a lot from the llvm's kaleidoscope tutorial
class Lexer {
public:
    enum Token {
        TOK_EOF = -1,
        TOK_ENDLINE = -2,
        
        TOK_IDENTIFIER = -3,
        TOK_NUMBER = -4,
        
        TOK_REPEAT = -5,
    };
    
public:
    Lexer(std::istream& input);
    
    int nextToken();
    
    const std::string& identifier() const;
    int number() const;
    
private:
    static const std::map<std::string, Token> KEYWORDS;
    static const char COMMENT;
    
    std::istream& m_input;
    
    char m_lastChar;
    
    std::string m_identifier;
    int m_number;
};

#endif /* defined(__dcpu__Lexer__) */
