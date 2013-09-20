//
//  VisitorsTests.h
//  dcpu
//
//  Created by Artur Mazurek on 19/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_VisitorsTests_h
#define dcpu_VisitorsTests_h

#include <bitset>
#include <sstream>

#include "CodegenVisitor.h"
#include "Instruction.h"
#include "Lexer.h"
#include "Parser.h"
#include "TestsUtil.h"

TESTS_START(VisitorsTests)

TEST {
    meta.name = "Instruction visitor simplest";
    
    std::stringstream s{"set a, 42"};
    
    Lexer l{s};
    Parser p;
    
    auto ast = p.parseCommand(l);
    
    CodegenVisitor::LabelsContainer labels;
    
    InstructionVisitor iv{labels};
    ast->b->expression->accept(iv);
    CHECK_EQUAL(iv.referencedRegister, "a", "Is instruction visitor properly finding a register");
    CHECK_EQUAL(iv.result(), 0, "Is result correctly 0");
    
    InstructionVisitor iv2{labels};
    ast->a->expression->accept(iv2);
    CHECK_EQUAL(iv2.referencedRegister, "", "Is no register referenced");
    CHECK_EQUAL(iv2.result(), 42, "Is result correctly 42");
},

TEST {
    meta.name = "Binop instructions with labels";
    
    std::stringstream s{"set [a+42], 12*12-myLabel"};
    
    Lexer l{s};
    Parser p;
    
    auto ast = p.parseCommand(l);
    
    CodegenVisitor::LabelsContainer labels{ {"myLabel", 44} };
    
    InstructionVisitor iv{labels};
    ast->b->expression->accept(iv);
    CHECK_EQUAL(iv.referencedRegister, "a", "Is register a referenced correctly");
    CHECK_EQUAL(iv.result(), 42, "Is offset added correctly");
    
    CodegenVisitor::LabelsContainer noLabels;
    InstructionVisitor ivNoLabels{noLabels};
    ast->a->expression->accept(ivNoLabels);
    REQUIRE_EQUAL(ivNoLabels.unresolvedLabels.size(), 1, "There must be one unresolved label");
    CHECK_EQUAL(ivNoLabels.unresolvedLabels[0], "myLabel", "Is the unresolved label found properly");
    
    InstructionVisitor iv2{labels};
    ast->a->expression->accept(iv2);
    CHECK_TRUE(iv2.unresolvedLabels.empty(), "Are the labels properly resolved");
    CHECK_EQUAL(iv2.result(), 100, "Is the expression calculated correctly when label is known");
},

TEST {
    meta.name = "More labels";
    
    std::stringstream s{"set a+one+three*four, sp-one+three*four"};
    
    Lexer l{s};
    Parser p;
    
    auto ast = p.parseCommand(l);
    
    CodegenVisitor::LabelsContainer someLabels{ {"one", 1}, {"three", 3} };
    CodegenVisitor::LabelsContainer allLabels{ {"one", 1}, {"three", 3}, {"four", 4} };
    
    InstructionVisitor ivB1{someLabels};
    ast->b->expression->accept(ivB1);
    CHECK_EQUAL(ivB1.referencedRegister, "a", "Is a referenced correctly");
    REQUIRE_EQUAL(ivB1.unresolvedLabels.size(), 1, "Are labels unrecognised");
    CHECK_EQUAL(ivB1.unresolvedLabels[0], "four", "Is proper label unrecognised");
    
    InstructionVisitor ivB2{allLabels};
    ast->b->expression->accept(ivB2);
    CHECK_EQUAL(ivB2.referencedRegister, "a", "Is a referenced correctly");
    CHECK_TRUE(ivB2.unresolvedLabels.empty(), "Are all labels resolved");
    CHECK_EQUAL(ivB2.result(), 13, "Is expression calculated correctly");
    
    InstructionVisitor ivA1{someLabels};
    ast->a->expression->accept(ivA1);
    CHECK_EQUAL(ivA1.referencedRegister, "sp", "Is proper register referenced");
    REQUIRE_EQUAL(ivA1.unresolvedLabels.size(), 1, "Are labels unrecognised");
    CHECK_EQUAL(ivA1.unresolvedLabels[0], "four", "Is proper label unrecognised");
    
    InstructionVisitor ivA2{allLabels};
    ast->a->expression->accept(ivA2);
    CHECK_EQUAL(ivA2.referencedRegister, "sp", "Is proper register referenced");
    CHECK_TRUE(ivA2.unresolvedLabels.empty(), "No labels should be unresolved");
    CHECK_EQUAL(ivA2.result(), 11, "Is expression calculated correctly 2");
},

TEST {
    meta.name = "Codegen visitor";
    
    std::stringstream s{"set a, 1"};
    Instruction i{ OP_SET, REG_A, 0x22 };
    
    Lexer l{s};
    Parser p;
    
    auto ast = p.parseCommand(l);
    
    CodegenVisitor::LabelsContainer noLabels{};
    CodegenVisitor cv{noLabels};
    
    ast->accept(cv);
    
    REQUIRE_EQUAL(cv.assembled.size(), 1, "Should assemble to one instruction");
    CHECK_TRUE(cv.assembled[0].code.first, "Should be fully generated");
    uint16_t val = i.raw[0] << 8;
    val += i.raw[1];
    
    CHECK_EQUAL(cv.assembled[0].code.second, val, "Is value correctly calculated");
},

TESTS_END

#endif
