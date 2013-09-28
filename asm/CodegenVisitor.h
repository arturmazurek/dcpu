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
#include <functional>
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
    typedef std::map<std::string, uint16_t> LabelsContainer;
    static LabelsContainer NoLabels;
public:
    std::vector<Assembler::CodeLine> assembled;
    
    CodegenVisitor(const LabelsContainer& labels = NoLabels);
    
    virtual void visit(CommandExprAST& command) override;
    
private:
    static const std::map<std::string, std::function<bool(CodegenVisitor*, CommandExprAST&)>> PSEUDO_OPCODE_FUNCTIONS;
    
    bool tryPseudoOpcodes(CommandExprAST& from);
    bool tryBaseOpcodes(CommandExprAST& from);
    bool trySpecialOpcodes(CommandExprAST& from);
    
    std::pair<uint8_t, std::unique_ptr<ExprAST>> codegenOperand(OperandExprAST& from) const;
    uint16_t makeInstruction(uint8_t a, uint8_t b, uint8_t o) const;
    uint16_t makeInstruction(uint8_t a, uint8_t o) const;
    
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
    
    InstructionVisitor(const CodegenVisitor::LabelsContainer& labels = CodegenVisitor::NoLabels);
    
    int result() const;
    
    virtual void visit(BinaryExprAST& node) override;
    virtual void visit(IdentifierExprAST& node) override;
    virtual void visit(NumberExprAST& node) override;
    
private:
    const CodegenVisitor::LabelsContainer& m_labels;
    std::stack<int> m_values;
};

#endif /* defined(__dcpu__CodegenVisitor__) */
