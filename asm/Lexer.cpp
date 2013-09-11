//
//  Lexer.cpp
//  dcpu
//
//  Created by Artur Mazurek on 11/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Lexer.h"

#include <cctype>

Lexer::Lexer(Lexer::input_iterator input) : m_input{input}, m_lastChar{' '} {
    
}

const std::string& Lexer::identifier() const {
    return m_identifier;
}

Lexer::Token Lexer::nextToken() {
    while(isspace(m_lastChar)) {
        m_lastChar = *m_input;
        ++m_input;
    }
    
    // identifier: [a-zA-Z][a-zA-Z0-9]*
    if(isalpha(m_lastChar)) {
        m_identifier = m_lastChar;
        while(m_input != input_iterator{}) {
            m_identifier += *m_input;
            ++m_input;
        }
        return TOK_IDENTIFIER;
    }
    
    // digit - [1-9][0-9]*
    if(isdigit(m_lastChar)) {
        
    }
    
    return TOK_EOF;
}