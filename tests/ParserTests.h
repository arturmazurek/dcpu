//
//  ParserTests.h
//  dcpu
//
//  Created by Artur Mazurek on 12/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_ParserTests_h
#define dcpu_ParserTests_h

#include <sstream>

#include "ASTVisitor.h"
#include "Lexer.h"
#include "Parser.h"
#include "TestsUtil.h"

TESTS_START(ParserTests)

TEST {
    meta.name = "Just label";

    std::stringstream s{"label:   \n\n "};
    Lexer l{s};
    Parser p;

    auto ast = p.parseCommand(l);

    REQUIRE_TRUE(ast->label != nullptr, "Is label properly filled");
    CHECK_EQUAL(ast->label->identifier, "label", "Is label retrieved properly");
    
    CHECK_TRUE(ast->op == nullptr, "Is op correctly empty");
    CHECK_TRUE(ast->a == nullptr, "Is a correctly empty");
    CHECK_TRUE(ast->b == nullptr, "Is b correctly empty");
},

TEST {
    meta.name = "Just operation";
    
    std::stringstream s{"  op   "};
    Lexer l{s};
    Parser p;
    
    auto ast = p.parseCommand(l);
    
    REQUIRE_TRUE(ast->op != nullptr, "Is operation properly filled");
    CHECK_EQUAL(ast->op->identifier, "op", "Is label retrieved properly");
    
    CHECK_TRUE(ast->label == nullptr, "Is label correctly empty");
    CHECK_TRUE(ast->a == nullptr, "Is a correctly empty");
    CHECK_TRUE(ast->b == nullptr, "Is b correctly empty");
},

TEST {
    meta.name = "Base parsing";
    
    std::stringstream s{"aaaa: dddd"};
    Lexer l{s};
    Parser p;
    
    auto ast = p.parseCommand(l);
    
    REQUIRE_TRUE(ast->label != nullptr, "Is label properly found");
    CHECK_EQUAL(ast->label->identifier, "aaaa", "Is label retrieved properly");
    
    REQUIRE_TRUE(ast->op != nullptr, "Is operation properly found");
    CHECK_EQUAL(ast->op->identifier, "dddd", "Is operation retrieved properly");
    
    CHECK_TRUE(ast->a == nullptr, "Is operand a correctly empty");
    CHECK_TRUE(ast->b == nullptr, "Is operand b correctly empty");
},

TEST {
    meta.name = "No comma";
    
    std::stringstream s{"op a b"};
    Lexer l{s};
    Parser p;
    
    bool caught = false;
    try {
        auto ast = p.parseCommand(l);
    } catch(const ParserException& e) {
        caught = true;
    }
    
    CHECK_TRUE(caught, "An exception must be thrown when there's no comma");
},

TEST {
    meta.name = "Full line parsing";
    
    std::stringstream s{
        "label1: op1 a, [b] \n"\
        "label2: op2 [c],d"
    };
    
    Lexer l{s};
    Parser p;
    
    auto ast = p.parseCommand(l);
    
    REQUIRE_TRUE(ast->label != nullptr, "Is label properly found");
    CHECK_EQUAL(ast->label->identifier, "label1", "Is label retrieved properly");
    
    REQUIRE_TRUE(ast->op != nullptr, "Is operation properly found");
    CHECK_EQUAL(ast->op->identifier, "op1", "Is operation retrieved properly");
    
    REQUIRE_TRUE(ast->a != nullptr, "Is operator a properly found");
    CHECK_TRUE(!(ast->a->addressing), "Is operator a non-addressing");

    REQUIRE_TRUE(ast->b != nullptr, "Is operator b properly found");
    CHECK_TRUE(ast->b->addressing, "Is operator b addressing");
    
    ast = p.parseCommand(l);
    
    REQUIRE_TRUE(ast->label != nullptr, "Is label properly found 2");
    CHECK_EQUAL(ast->label->identifier, "label2", "Is label retrieved properly 2");
    
    REQUIRE_TRUE(ast->op != nullptr, "Is operation properly found 2");
    CHECK_EQUAL(ast->op->identifier, "op2", "Is operation retrieved properly 2");
    
    REQUIRE_TRUE(ast->a != nullptr, "Is operator c properly found");
    CHECK_TRUE(ast->a->addressing, "Is operator c non-addressing");
    
    REQUIRE_TRUE(ast->b != nullptr, "Is operator d properly found");
    CHECK_TRUE(!(ast->b->addressing), "Is operator d non-addressing");
},

TEST {
    meta.name = "Binary ops";
    
    class TestVisitorA :
    public ASTVisitor,
    public ASTVisitorType<IdentifierExprAST>,
    public ASTVisitorType<NumberExprAST>,
    public ASTVisitorType<OperandExprAST>,
    public ASTVisitorType<BinaryExprAST> {
    public:
        TestVisitorA() : count{0} {}
        int count;
        
        virtual void visit(IdentifierExprAST& node) override {
            ++count;
            CHECK_EQUAL(node.identifier, "a", "Is a found");
        }
    
        virtual void visit(NumberExprAST& node) override {
            ++count;
            CHECK_EQUAL(node.value, 1, "Is 1 properly found");
        }
    
        virtual void visit(BinaryExprAST& node) override {
            ++count;
            CHECK_EQUAL(node.binop, '+', "Is the binary operation an addition");
            REQUIRE_TRUE(node.lhs != nullptr, "Left hand must be parsed");
            REQUIRE_TRUE(node.rhs != nullptr, "Right hand must be parsed");
            node.lhs->accept(*this);
            node.rhs->accept(*this);
        }

        virtual void visit(OperandExprAST& node) override {
            ++count;
            CHECK_TRUE(node.addressing, "Is [a+1] addressing");
            REQUIRE_TRUE(node.expression != nullptr, "Is there an expression");
            node.expression->accept(*this);
        }
    } visitorA;
    
    class TestVisitorB :
    public ASTVisitor,
    public ASTVisitorType<IdentifierExprAST>,
    public ASTVisitorType<OperandExprAST> {
    public:
        TestVisitorB() : count{0} {}
        int count;
        
        virtual void visit(IdentifierExprAST& node) override {
            ++count;
            CHECK_EQUAL(node.identifier, "b", "Is b found");
        }
        
        virtual void visit(OperandExprAST& node) override {
            ++count;
            CHECK_TRUE(!(node.addressing), "Is b non addressing");
            REQUIRE_TRUE(node.expression != nullptr, "Is there an expression 2");
            node.expression->accept(*this);
        }
    } visitorB;
    
    std::stringstream s{
        "op [a+1],b"
    };
    
    Lexer l{s};
    Parser p;
    
    auto ast = p.parseCommand(l);
    
    CHECK_TRUE(ast->label == nullptr, "Is there no label");
    
    REQUIRE_TRUE(ast->op != nullptr, "Is operation parsed");
    CHECK_EQUAL(ast->op->identifier, "op", "Is operation properly retrieved");
    
    REQUIRE_TRUE(ast->a != nullptr, "Is operand a retrieved");
    ast->a->accept(visitorA);
    CHECK_EQUAL(visitorA.count, 4, "Is visitorA visited appropriate number of times");

    REQUIRE_TRUE(ast->b != nullptr, "Is operand b retrieved");
    ast->b->accept(visitorB);
    CHECK_EQUAL(visitorB.count, 2, "Is visitor B visited appropriate number of times");
},

TEST {
    meta.name = "Arithmetic operations";
    
    class CountingVisitor :
    public ASTVisitor,
    public ASTVisitorType<BinaryExprAST>,
    public ASTVisitorType<NumberExprAST>,
    public ASTVisitorType<OperandExprAST> {
    public:
        int sum;
        CountingVisitor() : sum{0} {}
        
        virtual void visit(BinaryExprAST& node) override {
            REQUIRE_TRUE(node.lhs != nullptr, "Are left hand expressions parsed");
            REQUIRE_TRUE(node.rhs != nullptr, "Are right hand expressions parsed");
            
            CountingVisitor a;
            node.lhs->accept(a);
            CountingVisitor b;
            node.rhs->accept(b);
            
            switch(node.binop) {
                case '+':
                    sum += a.sum + b.sum;
                    break;
                    
                case '-':
                    sum += a.sum - b.sum;
                    break;
                    
                case '*':
                    sum += a.sum * b.sum;
                    break;
                    
                case '/':
                    sum += a.sum / b.sum;
                    break;
                    
                default:
                    assert(!"Should never get here");
                    
            }
        }
        virtual void visit(NumberExprAST& node) override {
            sum = node.value;
        }
        virtual void visit(OperandExprAST& node) override {
            node.expression->accept(*this);
        }
    };
    
    std::stringstream s{"op 1+2+3*4-2/1, [2*3*4*5 + 1]"};
    Lexer l{s};
    
    Parser p;
    
    auto ast = p.parseCommand(l);
    CountingVisitor v1;
    ast->a->accept(v1);
    CHECK_EQUAL(v1.sum, 13, "Are numbers counted correctly");

    CountingVisitor v2;
    ast->b->accept(v2);
    CHECK_EQUAL(v2.sum, 121, "Are numbers counted correctly 2");
    CHECK_TRUE(ast->b->addressing, "Is b correctly addressing");
},

TESTS_END

#endif
