//
//  CodegenVisitor.h
//  dcpu
//
//  Created by Artur Mazurek on 16.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __dcpu__CodegenVisitor__
#define __dcpu__CodegenVisitor__

#include <cstdint>
#include <string>
#include <vector>

#include "Assembler.h"

#include "ASTNodes.h"
#include "ASTVisitor.h"

class CodegenVisitor : public ASTVisitor,
public ASTVisitorType<CommandExprAST> {
public:
    std::vector<Assembler::CodeLine> assembled;
    std::string label;
    
    virtual void visit(CommandExprAST& command) override;
};

class InstructionVisitor : public ASTVisitor,
public ASTVisitorType<BinaryExprAST>,
public ASTVisitorType<IdentifierExprAST>,
public ASTVisitorType<NumberExprAST>,
public ASTVisitorType<OperandExprAST>{
public:
    bool addressing;
    uint8_t cmd;
    int nextWord;
    std::string label;
    
    InstructionVisitor() : addressing{false}, cmd{0}, nextWord{0} {}
    
    virtual void visit(BinaryExprAST& node) override;
    virtual void visit(IdentifierExprAST& node) override;
    virtual void visit(NumberExprAST& node) override;
};

#endif /* defined(__dcpu__CodegenVisitor__) */
