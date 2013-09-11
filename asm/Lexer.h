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

class Lexer {
public:
    enum Token {
        TOK_EOF = -1,
        
        TOK_IDENT = -2,
        TOK_NUMBER = -3
    };
    
public:
    Lexer(std::istream_iterator<char> input);
    
    Token nextToken();
    
private:
    bool isWhitespace(char c) const;
    
private:
    std::istream_iterator<char> m_input;
    
    char m_lastChar;
};

#endif /* defined(__dcpu__Lexer__) */
