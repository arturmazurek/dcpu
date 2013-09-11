//
//  Lexer.cpp
//  dcpu
//
//  Created by Artur Mazurek on 11/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Lexer.h"

Lexer::Lexer(std::istream_iterator<char> input) : m_input{input}, m_lastChar{' '} {
    
}

Lexer::Token Lexer::nextToken() {
    
}

bool Lexer::isWhitespace(char c) const {
    switch (c) {
        case ' ':
        case '\t':
            return true;
            
        default:
            return false;
    }
}