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
        
        assembled.push_back(Assembler::CodeLine{nullptr, {true, first}});
        if(a.second) {
            assembled.push_back(Assembler::CodeLine{std::move(a.second), {false, 0}});
        }
        if(b.second) {
            assembled.push_back(Assembler::CodeLine{std::move(b.second), {false, 0}});
        }
        
        return;
    }
    
    found = Constants::SPECIAL_OPCODES_NAMES.find(command.op->identifier);
    if(found != Constants::SPECIAL_OPCODES_NAMES.end()) {
        first |= (found->second & 0b11111) << 5;
        
        // codegen a
        auto a = codegenOperand(*command.a);
        first |= (a.first & 0b111111) << 10;
        
        assembled.push_back(Assembler::CodeLine{nullptr, {true, first}});
        if(a.second) {
            assembled.push_back(Assembler::CodeLine{std::move(a.second), {false, 0}});
        }
        
        return;
    }
    
    throw AssemblerException("Unknown operation - \"" + command.op->identifier + "\"");
}

std::pair<uint8_t, std::unique_ptr<ExprAST>> CodegenVisitor::codegenOperand(OperandExprAST& from) const {
//    return {0, nullptr};
    InstructionVisitor iv{m_labels};
    from.accept(iv);
//    return {iv.value, iv.done ? nullptr : from };
    
    std::pair<uint8_t, std::unique_ptr<ExprAST>> result;
//    result.first = iv.value;
//    result.second = iv.done ? nullptr : std::move(from.expression);
    
    bool requireNext = false;
    
    if(iv.referencedRegister.length()) {
        auto it = Constants::REGISTER_NAMES.find(iv.referencedRegister);
        assert(it != Constants::REGISTER_NAMES.end() && "Recognised as register, but not a register?");
        
        result.first = static_cast<uint8_t>(it->second);
        if(from.addressing && iv.value == 0) {
            result.first += Constants::ADDRESSING;
        } else if(from.addressing && iv.value != 0) {
            result.first += Constants::ADDRESSING_AND_NEXT_WORD;
            requireNext = true;
        }
    }
    if(iv.referencedRegister == "sp") {
        if(!from.addressing) {
            result.first = 0x1b;
        } else if(iv.done) {
            result.first = 0x19;
        } else {
            result.first = 0x1a;
            requireNext = true;
        }
    }
    if(iv.referencedRegister == "pc") {
        result.first = 0x1c;
    }
    if(iv.referencedRegister == "ex") {
        result.first = 0x1d;
    }
    
    if(iv.done) {
        if(iv.value >= -1 && iv.value <= 30 && !from.addressing) {
            result.first = static_cast<uint8_t>(iv.value);
        } else {
            result.first = Constants::NEXT_WORD_ADDR;
            requireNext = true;
        }
    } else {
        if(from.addressing) {
            result.first = Constants::NEXT_WORD_ADDR;
        } else {
            result.first = Constants::NEXT_WORD;
        }
        requireNext = true;
    }
    
    if(requireNext) {
        result.second = std::move(from.expression);
    }
    return result;
}

// Instruction visitor --------------------------------------------------------

InstructionVisitor::InstructionVisitor(const CodegenVisitor::LabelsContainer& labels) : done{false}, value{0}, m_labels{labels} {

}

void InstructionVisitor::visit(BinaryExprAST& node) {
    
}

void InstructionVisitor::visit(IdentifierExprAST& node) {
    
}

void InstructionVisitor::visit(NumberExprAST& node) {
    
}