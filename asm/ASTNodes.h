//
//  ASTNodes.h
//  dcpu
//
//  Created by Artur Mazurek on 12/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __dcpu__ASTNodes__
#define __dcpu__ASTNodes__

#include <memory>
#include <string>

// A lot of this is taken from llvm's Kaleidoscope tutorial

class ExprAST {
public:
    virtual ~ExprAST() {}
};

class NumberExprAST : public ExprAST {
public:
    NumberExprAST(int val) {}
};

class IdentifierExprAST : public ExprAST {
public:
    IdentifierExprAST(const std::string& ident) : m_ident{ident} {}
    
    const std::string& ident() const { return m_ident; }
    
private:
    std::string m_ident;
};

class OperandExprAST : public ExprAST {
public:
    OperandExprAST() : addressing{false} {}
    
public:
    std::unique_ptr<ExprAST> expression;
    bool addressing;
};

class BinaryExprAST : public ExprAST {
public:
    BinaryExprAST(int binop, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs) {}
};

class CommandExprAST : public ExprAST {
public:
    CommandExprAST() {}
    
    std::unique_ptr<IdentifierExprAST> label;
    std::unique_ptr<IdentifierExprAST> op;
    std::unique_ptr<OperandExprAST> a;
    std::unique_ptr<OperandExprAST> b;
};

#endif /* defined(__dcpu__ASTNodes__) */
