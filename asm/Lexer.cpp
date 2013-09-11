//
//  Lexer.cpp
//  dcpu
//
//  Created by Artur Mazurek on 11/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Lexer.h"

#include <cctype>

#include "LexerException.h"

Lexer::Lexer(std::istream& input) : m_input{input}, m_lastChar{' '}, m_number{0} {
    
}

const std::string& Lexer::identifier() const {
    return m_identifier;
}

int Lexer::number() const {
    return m_number;
}

Lexer::Token Lexer::nextToken() {    
    while(isspace(m_lastChar)) {
        m_lastChar = m_input.get();
    }
    
    // identifier: [a-zA-Z][a-zA-Z0-9]*
    if(isalpha(m_lastChar)) {
        m_identifier = m_lastChar;
        while(isalnum(m_lastChar = m_input.get())) {
            m_identifier += m_lastChar;
        }
        return TOK_IDENTIFIER;
    }
    
    // digit - [1-9][0-9]*
    if(isdigit(m_lastChar)) {
        if(m_lastChar == '0') {
            throw LexerException("Only the following format of digits - [1-9][0-9]* currently allowed - tried '0'");
        }
        std::string number;
        do {
            number += m_lastChar;
            m_lastChar = m_input.get();
        } while (isdigit(m_lastChar));
        
        m_number = std::stoi(number);
        return TOK_NUMBER;
    }
    
    return TOK_EOF;
}