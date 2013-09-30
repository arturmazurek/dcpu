//
//  Assembler.h
//  dcpu
//
//  Created by Artur Mazurek on 16.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __dcpu__Assembler__
#define __dcpu__Assembler__

#include <cstdint>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "Parser.h"
#include "Registers.h"

class CommandExprAST;
class Lexer;

class Assembler {
public:
    void setLexer(std::unique_ptr<Lexer> lexer);
    void assemble();
    const std::vector<uint16_t>& binary() const;
    
private:
    void parseSource();
    void resolveLabels();
    void createBinary();
    
private:
    friend class CodegenVisitor;
    
    static const std::string JUMP_IDENTIFIER;
    static const unsigned JUMP_LENGTH;
    
    Parser m_parser;
    std::unique_ptr<Lexer> m_lexer;
    
    std::map<std::string, uint16_t> m_labels;
    struct CodeLine {
        typedef std::pair<bool, uint16_t> CodeOptional;
        CodeLine(std::shared_ptr<ExprAST> generator, CodeOptional code) : generator{generator}, code{code} {}
        
        std::shared_ptr<ExprAST> generator;
        CodeOptional code;
    };
    std::vector<CodeLine> m_program;
    
    std::vector<uint16_t> m_binary;
};

#endif /* defined(__dcpu__Assembler__) */
