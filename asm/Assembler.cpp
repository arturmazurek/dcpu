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
#include "CodegenVisitor.h"
#include "Constants.h"
#include "Instruction.h"
#include "Parser.h"
#include "Lexer.h"

const std::string Assembler::JUMP_IDENTIFIER{"jmp"};
const unsigned Assembler::JUMP_LENGTH{2};

void Assembler::setLexer(std::unique_ptr<Lexer> lexer) {
    m_lexer = std::move(lexer);
}

const std::vector<uint16_t>& Assembler::binary() const {
    return m_binary;
}

void Assembler::assemble() {
    assert(m_lexer && "Lexer must be given to assemble");
    
    parseSource();
    resolveLabels();
    createBinary();
}

void Assembler::parseSource() {
    m_program.clear();
    m_labels.clear();
    
    while(m_lexer->good()) {
        auto node = m_parser.parseCommand(*m_lexer);
        
        if(node->label) {
            const auto& labelName = node->label->identifier;
            if(m_labels.find(labelName) != m_labels.end()) {
                throw AssemblerException("Duplicate label: \"" + labelName + "\"");
            }
            m_labels[labelName] = m_program.size();
        }
        
        // check if not just a line with label
        if(node->op) {
            CodegenVisitor v{m_labels};
            node->accept(v);
            m_program.insert(m_program.end(), v.assembled.begin(), v.assembled.end());
        }
    }
}

void Assembler::resolveLabels() {
    for(auto& line : m_program) {
        if(line.code.first) {
            continue; // code already generated
        }
        
        InstructionVisitor v{m_labels};
        line.generator->accept(v);
        if(v.unresolvedLabels.size()) {
            std::string s = "Unresolved labels: ";
            for(const auto& l : v.unresolvedLabels) {
                s += l + " ";
            }
            throw AssemblerException(s);
        }
        if(v.result() > std::numeric_limits<uint16_t>::max()) {
            throw AssemblerException("Too big value");
        }
        if(v.result() < std::numeric_limits<int16_t>::min()) {
            throw AssemblerException("Too big negative value");
        }
        
        line.code.second = static_cast<uint16_t>(v.result());
    }
}

void Assembler::createBinary() {
    m_binary.clear();
    
    for(auto& assembled : m_program) {
        m_binary.push_back(assembled.code.second);
    }
}
