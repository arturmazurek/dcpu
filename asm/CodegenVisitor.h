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
#include <map>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Assembler.h"

#include "ASTNodes.h"
#include "ASTVisitor.h"

class CodegenVisitor : public ASTVisitor,
public ASTVisitorType<CommandExprAST> {
public:
    std::vector<Assembler::CodeLine> assembled;
    
    typedef std::map<std::string, uint16_t> LabelsContainer;
    CodegenVisitor(const LabelsContainer& labels);
    
    virtual void visit(CommandExprAST& command) override;
    
private:
    std::pair<uint8_t, std::unique_ptr<ExprAST>> codegenOperand(OperandExprAST& from) const;
    
private:
    const LabelsContainer& m_labels;
};

class InstructionVisitor : public ASTVisitor,
public ASTVisitorType<BinaryExprAST>,
public ASTVisitorType<IdentifierExprAST>,
public ASTVisitorType<NumberExprAST> {
public:
    struct UnresolvedLabelException {
        UnresolvedLabelException(const std::string& label) : label{label} {}
        std::string label;
    };
    struct DuplicateRegisterException {
        DuplicateRegisterException(RegisterCode reg) : reg{reg} {}
        RegisterCode reg;
    };
    
    std::string referencedRegister;
    std::vector<std::string> unresolvedLabels;
    
    InstructionVisitor(const CodegenVisitor::LabelsContainer& labels);
    
    int result() const;
    
    virtual void visit(BinaryExprAST& node) override;
    virtual void visit(IdentifierExprAST& node) override;
    virtual void visit(NumberExprAST& node) override;
    
private:
    const CodegenVisitor::LabelsContainer& m_labels;
    std::stack<int> m_values;
};

#endif /* defined(__dcpu__CodegenVisitor__) */
