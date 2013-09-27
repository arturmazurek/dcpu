//
//  Parser.cpp
//  dcpu
//
//  Created by Artur Mazurek on 12/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Parser.h"

#include <sstream>

#include "ASMUtils.h"
#include "Lexer.h"
#include "ParserException.h"

void handleJMP(CommandExprAST& cmd) {
    cmd.op = std::make_unique<IdentifierExprAST>("set");
    // TODO
//    cmd.a = std::make_unique<OperandExprAST>();
//    cmd.a->expression = std::make_unique<IdentifierExprAST>("pc");
}

const std::map<int, std::function<void(CommandExprAST&)>> Parser::SPECIAL_TOKENS_FUNCTIONS{
    { Lexer::TOK_JMP, handleJMP }
};

const std::map<char, int> Parser::BINOP_PRECEDENCE {
    { '+', 10 },
    { '-', 10 },
    { '*', 20 },
    { '/', 20 }
};

Parser::Parser() : m_currentToken{0}, m_finished{false} {
    
}

std::unique_ptr<CommandExprAST> Parser::parseCommand(Lexer& l) {
    m_currentToken = l.nextToken();
    
    auto result = std::make_unique<CommandExprAST>(); 
    
    while (m_currentToken != Lexer::TOK_ENDLINE && m_currentToken != Lexer::TOK_EOF) {
        if(result->op) {
//            result->b = parseOperand(l);
//            if(m_currentToken == ',') {
//                m_currentToken = l.nextToken();
//                result->a = parseOperand(l);
//            } else if(m_currentToken == Lexer::TOK_ENDLINE || m_currentToken == Lexer::TOK_EOF) {
//                result->a = std::move(result->b);
//                break;
//            } else {
//                std::stringstream s;
//                s << "Expected comma or line end, found: '" << ((m_currentToken > 0) ? static_cast<char>(m_currentToken) : m_currentToken) << "'";
//                throw ParserException(s.str());
//            }
            result->operands = parseOperands(l);
        } else if(result->label) {
            result->op = parseIdentifier(l);
        } else {
            auto ident = parseIdentifier(l);
            if(m_currentToken == ':') {
                result->label = std::move(ident);
                m_currentToken = l.nextToken();
            } else {
                result->op = std::move(ident);
            }
        }
    }
    
    if(m_currentToken == Lexer::TOK_EOF) {
        m_finished = true;
    }
    
    return result;
}

std::unique_ptr<OperandExprAST> Parser::parseOperand(Lexer& l) {
    auto result = std::make_unique<OperandExprAST>();
    
    if(m_currentToken == '[') {
        result->addressing = true;
        m_currentToken = l.nextToken();
    }
    
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
    
    // token was consumed in parse expression
    
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
    
    auto result = std::make_unique<IdentifierExprAST>(l.identifier());
     
    // consume token
    m_currentToken = l.nextToken();
    
    return result;
}

std::unique_ptr<ExprAST> Parser::parseExpression(Lexer& l) {
    auto lhs = parsePrimary(l);
    return parseBinOpRhs(0, std::move(lhs), l);
}

std::unique_ptr<ExprAST> Parser::parsePrimary(Lexer& l) {
    switch (m_currentToken) {
        case Lexer::TOK_IDENTIFIER:
            return parseIdentifier(l);

        case Lexer::TOK_NUMBER:
            return parseNumber(l);
            
        case '(':
            return parseParenExpr(l);
            
        default: {
            std::stringstream s;
            s << "Unknown token '";
            if(m_currentToken < 0) {
                s << m_currentToken;
            } else {
                s << static_cast<char>(m_currentToken);
            }
            s << "' when expecting expression";
            throw ParserException(s.str());
        }
    }
}

int Parser::getTokenPrecedence(int token) const {
    if(!isascii(token)) {
        return -1;
    }
    
    auto found = BINOP_PRECEDENCE.find(token);
    if(found == BINOP_PRECEDENCE.end()) {
        return -1;
    } else {
        return found->second;
    }
}

std::unique_ptr<ExprAST> Parser::parseBinOpRhs(int precedence, std::unique_ptr<ExprAST> lhs, Lexer& l) {
    while(true) {
        int tokPrec = getTokenPrecedence(m_currentToken);
        
        if(tokPrec < precedence) {
            return std::move(lhs);
        }
        
        int binOp = m_currentToken;
        m_currentToken = l.nextToken();
        
        auto rhs = parsePrimary(l);
        if(!rhs) {
            return nullptr;
        }
        
        int nextPrecedence = getTokenPrecedence(m_currentToken);
        if(tokPrec < nextPrecedence) {
            rhs = parseBinOpRhs(tokPrec+1, std::move(rhs), l);
            if(!rhs) {
                return nullptr;
            }
        }
        
        lhs = std::make_unique<BinaryExprAST>(binOp, std::move(lhs), std::move(rhs));
    }
    
    return nullptr;
}

std::unique_ptr<NumberExprAST> Parser::parseNumber(Lexer& l) {
    if(m_currentToken != Lexer::TOK_NUMBER) {
        std::stringstream s;
        s << "Expected number, found token - '";
        if(m_currentToken < 0) {
            s << m_currentToken;
        } else {
            s << static_cast<char>(m_currentToken);
        }
        s << "'";
        throw ParserException{s.str()};
    }
    
    auto result = std::make_unique<NumberExprAST>(l.number());
    
    // consume token
    m_currentToken = l.nextToken();
    
    return result;
}

std::unique_ptr<ExprAST> Parser::parseParenExpr(Lexer& l) {
    // TODO
    
    m_currentToken = l.nextToken(); // eat '('
    auto expr = parseExpression(l);
    if(!expr) {
        return nullptr;
    }
    
    if(m_currentToken != ')') {
        std::stringstream s;
        s << "Expected ')' found: '";
        s << (m_currentToken > 0 ? static_cast<char>(m_currentToken) : m_currentToken);
        s << "'";
        throw ParserException(s.str());
    }
    
    // eat ')'
    m_currentToken = l.nextToken();
    
    return expr;
}

std::vector<std::unique_ptr<OperandExprAST>> Parser::parseOperands(Lexer& l) {
    std::vector<std::unique_ptr<OperandExprAST>> result;
    
    while (true) {
        result.emplace_back(parseOperand(l));
        if(m_currentToken == Lexer::TOK_ENDLINE || m_currentToken == Lexer::TOK_EOF) {
            return result;
        }
        
        if(m_currentToken == ',') {
            m_currentToken = l.nextToken(); // eat ','
        } else {
            throw ParserException("Unknown token encountered, expected ','");
        }
    }
    
    return result;
}
