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
    simplifyOperands();
    assembleProgram();
    resolveJumps();
    createBinary();
}

void Assembler::parseSource() {
    m_assembled.clear();
    m_labels.clear();
    
    while(m_lexer->good()) {
        auto node = m_parser.parseCommand(*m_lexer);
        auto assembled = std::make_unique<Assembled>(std::move(node));
        if(node->label) {
            if(m_labels.find(node->label->identifier) != m_labels.end()) {
                throw AssemblerException("Duplicate label found: \"" + node->label->identifier + "\"");
            }
            if(Constants::REGISTER_NAMES.find(node->label->identifier) != Constants::REGISTER_NAMES.end()) {
                throw AssemblerException("Label named as register found: \"" + node->label->identifier + "\""); 
            }
            m_labels[node->label->identifier] = assembled.get();
        }
        
        m_assembled.emplace_back(std::move(assembled));
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

void Assembler::createBinary() {
    m_binary.clear();
    
    for(auto& assembled : m_assembled) {
        m_binary.insert(m_binary.end(), assembled->instructions.begin(), assembled->instructions.end());
    }
}

//class JumpVisitor : public ASTVisitor,
//public ASTVisitorType<IdentifierExprAST>,
//public ASTVisitorType<NumberExprAST>,
//public ASTVisitorType<OperandExprAST> {
//public:
//    JumpVisitor() : addressing{false}, cmd{0}, nextWord{std::numeric_limits<int>::min()} {}
//    
//    uint8_t cmd;
//    int nextWord;
//    
//private:
//    bool addressing;
//    
//    virtual void visit(IdentifierExprAST& node) override {
//        
//    }
//    virtual void visit(NumberExprAST& node) override {
//        if(node.value > std::numeric_limits<uint16_t>::max()) {
//            throw AssemblerException("Too big number in operation");
//        }
//        if(node.value >= -1 && node.value <= 30) {
//            if(!addressing) {
//                cmd = static_cast<uint8_t>(0x21 + node.value);
//            } else {
//                cmd = Constants::NEXT_WORD;
//                nextWord = node.value;
//            }
//        } else {
//            
//        }
//    }
//    virtual void visit(OperandExprAST& node) override {
//        addressing = node.addressing;
//        node.expression->accept(*this);
//    }
//};

//void Assembler::checkJump(Assembled& from) {
//    Opcode op = OP_SET;
//    Registers::Code b = Registers::REG_PC;
//    int a = 0;
//     jmpVisitor;
//    from.parsed->a->accept(jmpVisitor);
////    const auto& jmpTarget = from.parsed->a;
////    if (jmpTarget->) {
////    }
//}

void Assembler::codegen(Assembled& source) {
    CodegenVisitor v;
    source.parsed->accept(v);
    if(v.label.size()) {
        source.jumpsTo = m_labels[v.label];
    }
}

void Assembler::simplify(OperandExprAST* node) const {
    
}

