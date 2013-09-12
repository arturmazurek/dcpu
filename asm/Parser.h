//
//  Parser.h
//  dcpu
//
//  Created by Artur Mazurek on 12/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __dcpu__Parser__
#define __dcpu__Parser__

#include <memory>

#include "ASTNodes.h"
#include "ParserException.h"

class Lexer;

class Parser {
public:
    Parser();
    ~Parser();
    
    std::unique_ptr<CommandExprAST> parseCommand(Lexer& l);
    
private:
    std::unique_ptr<OperandExprAST> parseOperand(Lexer& l);
    std::unique_ptr<IdentifierExprAST> parseIdentifier(Lexer& l);
    std::unique_ptr<ExprAST> parseExpression(Lexer& l);
    
private:
    int m_currentToken;
};

#endif /* defined(__dcpu__Parser__) */
