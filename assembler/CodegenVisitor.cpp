//
//  CodegenVisitor.cpp
//  dcpu
//
//  Created by Artur Mazurek on 16.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "CodegenVisitor.h"

#include "ASMUtils.h"
#include "AssemblerException.h"
#include "Constants.h"
#include "Instruction.h"
#include "Opcodes.h"

CodegenVisitor::LabelsContainer CodegenVisitor::NoLabels{};

static bool handleJMP(CodegenVisitor* thiz, CommandExprAST& ast) {
    ast.op = std::make_unique<IdentifierExprAST>("set");
    ast.operands.emplace(ast.operands.begin(), std::make_unique<OperandExprAST>());
    ast.operands[0]->expression = std::make_unique<IdentifierExprAST>("pc");
    return false; // this is a type of a preprocessor step actually
}

static bool handleRESW(CodegenVisitor* thiz, CommandExprAST& ast) {
    if(ast.operands.size() != 1) {
        throw AssemblerException("Must be one operands for resw");
    }
    
    InstructionVisitor iv{thiz->labels};
    ast.operands[0]->expression->accept(iv);
    
    if(!iv.referencedRegister.empty()) {
        throw AssemblerException("Cannot reference registers when calling resw");
    }
    if(!iv.unresolvedLabels.empty()) {
        throw AssemblerException("Cannot use unknown labels with resw");
    }
    if(iv.result() < 0) {
        throw AssemblerException("Cannot reserve negative amount of words");
    }
    if(iv.result() > std::numeric_limits<uint16_t>::max()) {
        throw AssemblerException("Trying to reserve more memory than total available");
    }
    
    for(int i = 0; i < iv.result(); ++i) {
        thiz->assembled.emplace_back(nullptr, std::make_pair(true, 0));
    }
    
    return true;
}

bool handleDW(CodegenVisitor* thiz, CommandExprAST& ast) {
    for(const auto& operandPtr : ast.operands) {
        InstructionVisitor iv;
        operandPtr->expression->accept(iv);
        
        if(iv.unresolvedLabels.size()) {
            throw AssemblerException("Cannot declare words with unknown label in them");
        }
        if(!iv.referencedRegister.empty()) {
            throw AssemblerException("Cannot reference register in declaration");
        }
        if(iv.result() > std::numeric_limits<uint16_t>::max()) {
            throw AssemblerException("Too big number used in declaration");
        }
        if(iv.result() < std::numeric_limits<int16_t>::min()) {
            throw AssemblerException("Too big negative number used in declaration");
        }
        
        thiz->assembled.emplace_back(nullptr, std::make_pair(true, iv.result()));
    }
    
    return true;
}

const std::map<std::string, std::function<bool(CodegenVisitor*, CommandExprAST&)>> CodegenVisitor::PSEUDO_OPCODE_FUNCTIONS {
    { "jmp", handleJMP },
    { "resw", handleRESW },
    { "dw", handleDW }
};

// Codegen visitor ------------------------------------------------------------

CodegenVisitor::CodegenVisitor(const LabelsContainer& labels) : labels{labels} {
}

void CodegenVisitor::visit(CommandExprAST& command) {
    if(tryPseudoOpcodes(command)) {
        return;
    }
    
    if(tryBaseOpcodes(command)) {
        return;
    }
    
    if(trySpecialOpcodes(command)) {
        return;
    }
    
    throw AssemblerException("Unknown operation - \"" + command.op->identifier + "\"");
}

bool CodegenVisitor::tryPseudoOpcodes(CommandExprAST& command) {
    auto found = PSEUDO_OPCODE_FUNCTIONS.find(command.op->identifier);
    if(found == PSEUDO_OPCODE_FUNCTIONS.end()) {
        return false;
    } else {
        return found->second(this, command);
    }
}

bool CodegenVisitor::tryBaseOpcodes(CommandExprAST& command) {
    auto found = Constants::OPCODES_NAMES.find(command.op->identifier);
    if(found == Constants::OPCODES_NAMES.end()) {
        return false;
    }
        
    if(command.operands.size() != 2) {
        throw ParserException("Wrong number of operands found for operation \"" + command.op->identifier + "\"");
    }
    
    // codegen a and b
    auto b = codegenOperand(*command.operands[0]);
    auto a = codegenOperand(*command.operands[1]);
    
    assembled.emplace_back(nullptr, std::make_pair(true, makeInstruction(a.first, b.first, found->second)));
    if(a.second) {
        InstructionVisitor iv{labels};
        a.second->accept(iv);
        
        if(iv.unresolvedLabels.empty()) {
            assembled.emplace_back(nullptr, std::make_pair(true, iv.result()));
        } else {
            assembled.emplace_back(std::move(a.second), std::make_pair(false, 0));
        }
    }
    if(b.second) {
        InstructionVisitor iv{labels};
        b.second->accept(iv);
        
        if(iv.unresolvedLabels.empty()) {
            assembled.emplace_back(nullptr, std::make_pair(true, iv.result()));
        } else {
            assembled.emplace_back(std::move(b.second), std::make_pair(false, 0));
        }
    }
    
    return true;
}

bool CodegenVisitor::trySpecialOpcodes(CommandExprAST& command) {
    auto found = Constants::SPECIAL_OPCODES_NAMES.find(command.op->identifier);
    if(found == Constants::SPECIAL_OPCODES_NAMES.end()) {
        return false;
    }
        
    if(command.operands.size() != 1) {
        throw ParserException("No operands found for operation \"" + command.op->identifier + "\"");
    }
    
    // codegen a
    auto a = codegenOperand(*command.operands[0]);
    
    assembled.emplace_back(nullptr, std::make_pair(true, makeInstruction(a.first, found->second)));
    if(a.second) {
        InstructionVisitor iv{labels};
        a.second->accept(iv);
        
        if(iv.unresolvedLabels.empty()) {
            assembled.emplace_back(nullptr, std::make_pair(true, iv.result()));
        } else {
            assembled.emplace_back(std::move(a.second), std::make_pair(false, 0));
        }
    }
    
    return true;
}

uint16_t CodegenVisitor::makeInstruction(uint8_t a, uint8_t b, uint8_t o) const {
    Instruction i{static_cast<Opcode>(o), b, a};
    return *reinterpret_cast<uint16_t*>(i.raw);
}

uint16_t CodegenVisitor::makeInstruction(uint8_t a, uint8_t o) const {
    Instruction i{static_cast<Opcode>(o), a};
    return *reinterpret_cast<uint16_t*>(i.raw);
}

std::pair<uint8_t, std::unique_ptr<ExprAST>> CodegenVisitor::codegenOperand(OperandExprAST& from) const {
    InstructionVisitor iv{labels};
    
    try {
        from.expression->accept(iv);
    } catch (const InstructionVisitor::DuplicateRegisterException& e) {
        throw ParserException("Duplicate register referenced"); // todo put the name
    } 
    bool done = iv.unresolvedLabels.size() == 0;
    
    std::pair<uint8_t, std::unique_ptr<ExprAST>> result;

    bool requireNext = false;
    
    if(iv.referencedRegister.length()) {
        auto it = Constants::REGISTER_NAMES.find(iv.referencedRegister);
        assert(it != Constants::REGISTER_NAMES.end() && "Recognised as register, but not a register?");
        
        result.first = static_cast<uint8_t>(it->second);
        if(from.addressing && iv.result() == 0) {
            result.first += Constants::ADDRESSING;
        } else if(from.addressing && iv.result() != 0) {
            result.first += Constants::ADDRESSING_AND_NEXT_WORD;
            requireNext = true;
        }
    }
    if(iv.referencedRegister == "sp") {
        if(!from.addressing) {
            if(iv.result() != 0) {
                throw ParserException("Cannot reference SP's value with a number");
            }
            result.first = REG_SP;
        } else if(done && iv.result() == 0) {
            result.first = Constants::PEEK;
        } else {
            result.first = Constants::PICK;
            requireNext = true;
        }
    }
    if(iv.referencedRegister == "pc") {
        result.first = REG_PC;
        if(iv.result() != 0) {
            throw ParserException("Cannot reference SP's value with a number");
        }
    }
    if(iv.referencedRegister == "ex") {
        result.first = REG_EX;
        if(iv.result() != 0) {
            throw ParserException("Cannot reference SP's value with a number");
        }
    }
    
    if(done) {
        if(iv.referencedRegister.empty()) {
            if(iv.result() >= -1 && iv.result() <= 30 && !from.addressing) {
                result.first = static_cast<uint8_t>(Constants::LITERALS_MINUS_1 + 1 + iv.result());
            } else {
                result.first = from.addressing ? Constants::NEXT_WORD_ADDR : Constants::NEXT_WORD;
                requireNext = true;
            }
        }
    } else {
        if(iv.referencedRegister.empty()) {
            if(from.addressing) {
                result.first = Constants::NEXT_WORD_ADDR;
            } else {
                result.first = Constants::NEXT_WORD;
            }
        } else {
            result.first += Constants::ADDRESSING_AND_NEXT_WORD;
        }
        requireNext = true;
    }
    
    if(requireNext) {
        result.second = std::move(from.expression);
    }
    return result;
}

// Instruction visitor --------------------------------------------------------

InstructionVisitor::InstructionVisitor(const CodegenVisitor::LabelsContainer& labels) : m_labels{labels} {

}

int InstructionVisitor::result() const {
    return m_values.top();
}

void InstructionVisitor::visit(BinaryExprAST& node) {
    node.lhs->accept(*this);
    node.rhs->accept(*this);
    
    // lvalue is visited first, so rvalue is popped first
    int rValue = m_values.top(); m_values.pop();
    int lValue = m_values.top(); m_values.pop();
    switch (node.binop) {
        case '+': m_values.push(lValue + rValue); break;
        case '-': m_values.push(lValue - rValue); break;
        case '*': m_values.push(lValue * rValue); break;
        case '/': m_values.push(lValue / rValue); break;
            
        default:
            assert(!"Should never get here");
            break;
    }
}

void InstructionVisitor::visit(IdentifierExprAST& node) {
    if(Constants::REGISTER_NAMES.find(node.identifier) != Constants::REGISTER_NAMES.end()) {
        // it's a register referenced
        if(!referencedRegister.empty() && referencedRegister != node.identifier) {
            throw DuplicateRegisterException(Constants::REGISTER_NAMES.at(node.identifier));
        }
        referencedRegister = node.identifier;
        m_values.push(0);
    } else if(m_labels.find(node.identifier) != m_labels.end()) {
        // use the actual number
        m_values.push(m_labels.at(node.identifier));
    } else {
        unresolvedLabels.push_back(node.identifier);
        m_values.push(0);
    }
}

void InstructionVisitor::visit(NumberExprAST& node) {
    m_values.push(node.value);
}