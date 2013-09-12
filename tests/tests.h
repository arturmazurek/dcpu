//
//  tests.h
//  dcpu
//
//  Created by Artur Mazurek on 23.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_tests_h
#define dcpu_tests_h

#include "BaseTests.h"
#include "CoreTests.h"
#include "LexerTests.h"
#include "OpcodesTests.h"
#include "ParserTests.h"
#include "SpecialOpcodesTests.h"
#include "TestsHolder.h"

static void runTests() {
    ADD_TESTS(BaseTests);
    ADD_TESTS(OpcodesTests);
    ADD_TESTS(SpecialOpcodesTests);
    ADD_TESTS(CoreTests);
    
    ADD_TESTS(LexerTests);
    ADD_TESTS(ParserTests);
    
    TestsHolder::instance().runTests();
}

#endif
