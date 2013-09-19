//
//  VisitorsTests.h
//  dcpu
//
//  Created by Artur Mazurek on 19/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_VisitorsTests_h
#define dcpu_VisitorsTests_h

#include <sstream>

#include "CodegenVisitor.h"
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
    ast->a->expression->accept(iv);
    CHECK_EQUAL(iv.referencedRegister, "a", "Is instruction visitor properly finding a register");
    CHECK_EQUAL(iv.result(), 0, "Is result correctly 0");
    
    InstructionVisitor iv2{labels};
    ast->b->expression->accept(iv2);
    CHECK_EQUAL(iv2.referencedRegister, "", "Is no register referenced");
    CHECK_EQUAL(iv2.result(), 42, "Is result correctly 42");
},

TESTS_END

#endif
