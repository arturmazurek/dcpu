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
#include <string>

// This takes a lot from the llvm's kaleidoscope tutorial
class Lexer {
public:
    enum Token {
        TOK_EOF = -1,
        
        TOK_IDENTIFIER = -2,
        TOK_NUMBER = -3
    };
    
public:
    typedef std::istream_iterator<char> input_iterator;
    Lexer(input_iterator input);
    
    Token nextToken();
    
    const std::string& identifier() const;
    
private:
    input_iterator m_input;
    
    char m_lastChar;
    
    std::string m_identifier;
};

#endif /* defined(__dcpu__Lexer__) */
