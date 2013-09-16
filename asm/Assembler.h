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
#include <vector>

#include "Parser.h"
#include "Registers.h"

class CommandExprAST;
class Lexer;

class Assembler {
public:
    void setLexer(std::unique_ptr<Lexer> lexer);
    void assemble();
    
private:
    struct Assembled;
    
    void parseSource();
    void simplifyOperands();
    void simplify(OperandExprAST* node) const;
    void assembleProgram();
    void resolveJumps();
    
    void codegen(Assembled& source);
    // Generates code for a jump (if to a code-static place)
    // or marks for future resolving if it's label jump
    void checkJump(Assembled& source);
    
private:
    struct Assembled {
        std::unique_ptr<CommandExprAST> parsed;
        std::vector<uint16_t> instructions;
        
        // If it is an unresolved jump (i.e. a label jump)
        Assembled* jumpsTo;
        unsigned size;
        uint16_t offset;
        
        Assembled(std::unique_ptr<CommandExprAST> from) : parsed{std::move(from)}, jumpsTo{nullptr}, size{0}, offset{0} {}
    };
    
    static const std::map<std::string, Registers::Code> REGISTER_NAMES;
    static const std::string JUMP_IDENTIFIER;
    static const unsigned JUMP_LENGTH;
    
    Parser m_parser;
    std::unique_ptr<Lexer> m_lexer;
    std::map<std::string, CommandExprAST*> m_labels;
    
    std::vector<std::unique_ptr<Assembled>> m_assembled;
};

#endif /* defined(__dcpu__Assembler__) */
