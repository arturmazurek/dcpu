//
//  Parser.cpp
//  dcpu
//
//  Created by Artur Mazurek on 12/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Parser.h"

#include <sstream>

#include "Lexer.h"
#include "ParserException.h"

namespace std {
    template <typename T, typename... Args>
    static std::unique_ptr<T> make_unique(Args&&... args){
        return std::unique_ptr<T>{new T(std::forward<Args>(args)...)};
    }
}

Parser::Parser() : m_currentToken{0} {
    
}

Parser::~Parser() {
    
}

std::unique_ptr<CommandExprAST> Parser::parseCommand(Lexer& l) {
    m_currentToken = l.nextToken();
    
    auto result = std::make_unique<CommandExprAST>(); 
    
    while (m_currentToken != Lexer::TOK_ENDLINE && m_currentToken != Lexer::TOK_EOF) {
        if (result->label && result->op && result->a) {
            result->b = parseOperand(l);
        } else if(result->label && result->op) {
            result->a = parseOperand(l);
        } else if(result->label) {
            result->op = parseIdentifier(l);
        } else {
            auto ident = parseIdentifier(l);
            m_currentToken = l.nextToken();
            if(m_currentToken == ':') {
                result->label = std::move(ident);
            }
        }
        
        m_currentToken = l.nextToken();
    }
    
    return result;
}

std::unique_ptr<OperandExprAST> Parser::parseOperand(Lexer& l) {
    auto result = std::make_unique<OperandExprAST>();
    
    if(m_currentToken == '[') {
        result->addressing = true;
        m_currentToken = l.nextToken();
    }
    
//    if(m_currentToken == Lexer::TOK_IDENTIFIER) {   
//        return std::make_unique<OperandExprAST>();
//    } else if(m_currentToken == Lexer::TOK_NUMBER) {
//        return std::make_unique<OperandExprAST>();
//    } else {
//        std::stringstream s;
//        s << "Expected identifier or number, found token - '";
//        if(m_currentToken < 0) {
//            s << m_currentToken;
//        } else {
//            s << static_cast<char>(m_currentToken);
//        }
//        s << "'";
//        throw ParserException{s.str()};
//    }
    result->expression = parseExpression(l);
    
    if(result->addressing) {
        if(m_currentToken == ']') {
            // consume ']'
            m_currentToken = l.nextToken();
        } else {
            std::stringstream s;
            s << "Expected ']', found: '" << (char)m_currentToken << "'";
            throw LexerException(s.str());
        }
    }
    
    return result;
}

std::unique_ptr<IdentifierExprAST> Parser::parseIdentifier(Lexer& l) {
    if(m_currentToken != Lexer::TOK_IDENTIFIER) {
        std::stringstream s;
        s << "Expected identifier, found token - '";
        if(m_currentToken < 0) {
            s << m_currentToken;
        } else {
            s << static_cast<char>(m_currentToken);
        }
        s << "'";
        throw ParserException{s.str()};
    }
    
    return std::make_unique<IdentifierExprAST>(l.identifier());
}

std::unique_ptr<ExprAST> Parser::parseExpression(Lexer& l) {
    return nullptr;
}
