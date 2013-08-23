//
//  tests.h
//  dcpu
//
//  Created by Artur Mazurek on 23.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_tests_h
#define dcpu_tests_h

#include "CoreTests.h"
#include "TestsHolder.h"

static void runTests() {
    ADD_TESTS(CoreTests);
    
    TestsHolder::instance().runTests();
}

#endif
