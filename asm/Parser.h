//
//  Parser.h
//  dcpu
//
//  Created by Artur Mazurek on 12/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __dcpu__Parser__
#define __dcpu__Parser__

#include <map>
#include <memory>

#include "ASTNodes.h"
#include "ParserException.h"

class Lexer;

class Parser {
public:
    Parser();
    
    std::unique_ptr<CommandExprAST> parseCommand(Lexer& l);
    
private:
    std::unique_ptr<OperandExprAST> parseOperand(Lexer& l);
    std::unique_ptr<IdentifierExprAST> parseIdentifier(Lexer& l);
    std::unique_ptr<ExprAST> parseExpression(Lexer& l);
    std::unique_ptr<ExprAST> parsePrimary(Lexer& l);
    std::unique_ptr<ExprAST> parseBinOpRhs(int precedence, std::unique_ptr<ExprAST> lhs, Lexer& l);
    std::unique_ptr<NumberExprAST> parseNumber(Lexer& l);
    std::unique_ptr<ExprAST> parseParenExpr(Lexer& l);
    
    std::unique_ptr<ExprAST> error(const std::string& what);
    
    int getTokenPrecedence(int token) const;
    
private:
    static std::map<char, int> BINOP_PRECEDENCE;
    
    int m_currentToken;
    bool m_finished;
};

#endif /* defined(__dcpu__Parser__) */
