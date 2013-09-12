//
//  Lexer.cpp
//  dcpu
//
//  Created by Artur Mazurek on 11/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Lexer.h"

#include <cctype>
#include <map>

#include "LexerException.h"

const std::map<std::string, Lexer::Token> Lexer::KEYWORDS{
    { "repeat", TOK_REPEAT }
};
const char Lexer::COMMENT{';'};

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
        
        auto it = KEYWORDS.find(m_identifier);
        if(it != KEYWORDS.end()) {
            return it->second;
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
    
    if(m_lastChar == COMMENT) {
        do {
            m_lastChar = m_input.get();
        } while (m_input && m_lastChar != '\n' && m_lastChar != '\r');
        
        if(m_input) {
            return nextToken();
        }
    }
    
    return TOK_EOF;
}