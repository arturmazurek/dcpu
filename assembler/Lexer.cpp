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

bool Lexer::good() const {
    return m_lastChar != TOK_EOF;
}

int Lexer::nextToken() {
    while(m_lastChar == ' ' || m_lastChar == '\t') {
        m_lastChar = m_input.get();
    }
    
    // OS X / Linux type endline
    if(m_lastChar == '\n') {
        m_lastChar = m_input.get();
        return TOK_ENDLINE;
    }
    
    // Windows type endline
    if(m_lastChar == '\r') {
        m_lastChar = m_input.get();
        if(m_lastChar == '\n') {
            m_lastChar = m_input.get();
        }
        return TOK_ENDLINE;
    }
    
    // identifier: [a-zA-Z][a-zA-Z0-9]*
    if(isalpha(m_lastChar)) {
        return tryIdentifier();
    }
    
    // digit - 0 | [1-9][0-9]*
    if(isdigit(m_lastChar)) {
        return tryDigit();
    }
    
    if(m_lastChar == COMMENT) {
        do {
            m_lastChar = m_input.get();
            
        } while(m_input && m_lastChar != '\n');
        
        if(!m_input) {
            return TOK_EOF;
        } else {
            return TOK_ENDLINE;
        }
    }
    
    if(!m_input) {
        return TOK_EOF;
    }
    
    char c = m_lastChar;
    m_lastChar = m_input.get();
    return c;
}

int Lexer::tryIdentifier() {
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

int Lexer::tryDigit() {
    if(m_lastChar == '0') {
        m_lastChar = m_input.get();
        if (isdigit(m_lastChar)) {
            throw LexerException("Only the following format of digits - 0 | [1-9][0-9]* currently allowed");
        }
        m_number = 0;
        return TOK_NUMBER;
    }
    std::string number;
    do {
        number += m_lastChar;
        m_lastChar = m_input.get();
    } while (isdigit(m_lastChar));
    
    m_number = std::stoi(number);
    return TOK_NUMBER;
}
