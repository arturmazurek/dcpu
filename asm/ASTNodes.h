//
//  ASTNodes.h
//  dcpu
//
//  Created by Artur Mazurek on 12/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __dcpu__ASTNodes__
#define __dcpu__ASTNodes__

#include <cassert>
#include <memory>
#include <string>

#include "ASTVisitor.h"

// A lot of this is taken from llvm's Kaleidoscope tutorial

class ExprAST {
public:
    virtual ~ExprAST() {}
    
    virtual void accept(ASTVisitor& visitor) {}
    
protected:
    template <class T>
    static void acceptVisitor(T& visited, ASTVisitor& visitor) {
        if(ASTVisitorType<T> *p = dynamic_cast<ASTVisitorType<T>*>(&visitor)) {
            p->visit(visited);
        } else {
            assert(!"Unhandled node type");
        }
    }
};

#define VISITABLE() \
virtual void accept(ASTVisitor &v) override \
{ return acceptVisitor(*this, v); }

class NumberExprAST : public ExprAST {
public:
    NumberExprAST(int value) : value{value} {}
    
    VISITABLE();
    
    const int value;
};

class IdentifierExprAST : public ExprAST {
public:
    IdentifierExprAST(const std::string& identifier) : identifier{identifier} {}
    
    VISITABLE();
    
    const std::string identifier;
};

class OperandExprAST : public ExprAST {
public:
    OperandExprAST() : addressing{false} {}
    
    VISITABLE();
    
public:
    std::unique_ptr<ExprAST> expression;
    bool addressing;
};

class BinaryExprAST : public ExprAST {
public:
    BinaryExprAST(int binop, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs) : binop{binop}, lhs{std::move(lhs)}, rhs{std::move(rhs)} {}
    
    VISITABLE();
    
public:
    const int binop;
    std::unique_ptr<ExprAST> lhs;
    std::unique_ptr<ExprAST> rhs;
};

class CommandExprAST : public ExprAST {
public:
    CommandExprAST() {}
    
    std::unique_ptr<IdentifierExprAST> label;
    std::unique_ptr<IdentifierExprAST> op;
    std::unique_ptr<OperandExprAST> a;
    std::unique_ptr<OperandExprAST> b;
};

#undef VISITABLE

#endif /* defined(__dcpu__ASTNodes__) */
