//
//  CodegenVisitor.cpp
//  dcpu
//
//  Created by Artur Mazurek on 16.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "CodegenVisitor.h"

#include "AssemblerException.h"
#include "Constants.h"
#include "Opcodes.h"

CodegenVisitor::LabelsContainer CodegenVisitor::NoLabels{};

// Codegen visitor ------------------------------------------------------------

CodegenVisitor::CodegenVisitor(const LabelsContainer& labels) : m_labels{labels} {
}

void CodegenVisitor::visit(CommandExprAST& command) {
    uint16_t first = 0;

    auto found = Constants::OPCODES_NAMES.find(command.op->identifier);
    if(found != Constants::OPCODES_NAMES.end()) {
        first |= (found->second & 0b11111);
        
        // codegen a and b
        auto a = codegenOperand(*command.a);
        first |= (a.first & 0b111111) << 10;
        
        auto b = codegenOperand(*command.b);
        first |= (b.first & 0b11111) << 5;
        
        assembled.emplace_back(nullptr, std::make_pair(true, first));
        if(a.second) {
            InstructionVisitor iv{m_labels};
            a.second->accept(iv);
            
            if(iv.unresolvedLabels.empty()) {
                assembled.emplace_back(nullptr, std::make_pair(true, iv.result()));
            } else {
                assembled.emplace_back(std::move(a.second), std::make_pair(false, 0));
            }
        }
        if(b.second) {
            InstructionVisitor iv{m_labels};
            b.second->accept(iv);
            
            if(iv.unresolvedLabels.empty()) {
                assembled.emplace_back(nullptr, std::make_pair(true, iv.result()));
            } else {
                assembled.emplace_back(std::move(b.second), std::make_pair(false, 0));
            }
        }
        
        return;
    }
    
    found = Constants::SPECIAL_OPCODES_NAMES.find(command.op->identifier);
    if(found != Constants::SPECIAL_OPCODES_NAMES.end()) {
        first |= (found->second & 0b11111) << 5;
        
        // codegen a
        auto a = codegenOperand(*command.a);
        first |= (a.first & 0b111111) << 10;
        
        assembled.emplace_back(nullptr, std::make_pair(true, first));
        if(a.second) {
            assembled.emplace_back(std::move(a.second), std::make_pair(false, 0));
        }
        
        return;
    }
    
    throw AssemblerException("Unknown operation - \"" + command.op->identifier + "\"");
}

std::pair<uint8_t, std::unique_ptr<ExprAST>> CodegenVisitor::codegenOperand(OperandExprAST& from) const {
    InstructionVisitor iv{m_labels};
    
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