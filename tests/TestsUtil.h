//
//  TestsUtil.h
//  dcpu
//
//  Created by Artur Mazurek on 23.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_TestsUtil_h
#define dcpu_TestsUtil_h

#include "TestsHolder.h"

struct TestAdder {
    TestsHolder::TestFunction f;
    
    TestAdder(TestsHolder::TestFunction f) : f{f} {}
    ~TestAdder() {
        TestsHolder::instance().addTest(f);
    }
};

#define D_TEST(l) \
TestAdder t##l = (TestsHolder::TestFunction)[](Core& core, bool& result)

#define TEST D_TEST(__LINE__)

#define CHECK_TRUE(x, msg) \
if(!(x)) {\
std::cout << "Test failed: " << msg << std::endl;\
result = false;\
}

#endif
