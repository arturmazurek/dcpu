//
//  Assembler.cpp
//  dcpu
//
//  Created by Artur Mazurek on 16.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Assembler.h"

#include <cassert>
#include <limits>

#include "ASMUtils.h"
#include "AssemblerException.h"
#include "Parser.h"
#include "Lexer.h"

const std::map<std::string, Registers::Code> Assembler::REGISTER_NAMES {
    { "a", Registers::REG_A },
    { "b", Registers::REG_B },
    { "c", Registers::REG_C },
    { "x", Registers::REG_X },
    { "y", Registers::REG_Y },
    { "z", Registers::REG_Z },
    { "i", Registers::REG_I },
    { "j", Registers::REG_J },
    { "sp", Registers::REG_SP },
    { "pc", Registers::REG_PC },
    { "ex", Registers::REG_EX },
};
const std::string Assembler::JUMP_IDENTIFIER{"jmp"};
const unsigned Assembler::JUMP_LENGTH{2};

void Assembler::setLexer(std::unique_ptr<Lexer> lexer) {
    m_lexer = std::move(lexer);
}

void Assembler::assemble() {
    assert(m_lexer && "Lexer must be given to assemble");
    
    parseSource();
    simplifyOperands();
    assembleProgram();
    resolveJumps();
}

void Assembler::parseSource() {
    m_assembled.clear();
    m_labels.clear();
    
    while(m_lexer->good()) {
        auto node = m_parser.parseCommand(*m_lexer);
        if(node->label) {
            if(m_labels.find(node->label->identifier) != m_labels.end()) {
                throw AssemblerException("Duplicate label found: \"" + node->label->identifier + "\"");
            }
            if(REGISTER_NAMES.find(node->label->identifier) != REGISTER_NAMES.end()) {
                throw AssemblerException("Label named as register found: \"" + node->label->identifier + "\""); 
            }
            m_labels[node->label->identifier] = node.get();
        }
        
        m_assembled.emplace_back(std::make_unique<Assembled>(std::move(node)));
    }
}

void Assembler::simplifyOperands() {
    for(auto& assembled : m_assembled) {
        if(assembled->parsed->a.get()) {
            simplify(assembled->parsed->a.get());
        }
        if(assembled->parsed->b) {
            simplify(assembled->parsed->b.get());
        }
    }
}

void Assembler::assembleProgram() {
    uint32_t currentOffset = 0;
    for(auto& assembled : m_assembled) {
        codegen(*assembled);
        
        assembled->offset = static_cast<uint16_t>(currentOffset);
        
        currentOffset += assembled->size;
        if(currentOffset > std::numeric_limits<uint16_t>::max()) {
            throw AssemblerException("Too big executable");
        }
        
    }
}

void Assembler::resolveJumps() {
    for(auto& assembled : m_assembled) {
        if(assembled->jumpsTo) {
            assembled->instructions.push_back(assembled->jumpsTo->offset);
        }
    }
}

void Assembler::checkJump(Assembled& from) {
    
}

void Assembler::codegen(Assembled& source) {
    if(source.parsed->op->identifier == JUMP_IDENTIFIER) {
        checkJump(source);
        return;
    }
    
    
}

void Assembler::simplify(OperandExprAST* node) const {
    
}

