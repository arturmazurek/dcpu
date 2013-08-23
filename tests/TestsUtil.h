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

#define ADD_TESTS(name) prepare##name()

#define TESTS_START(name) void prepare##name() {\
TestsHolder::TestFunction tests[] = {

#define TESTS_END };\
for(int i = 0; i < ARRAY_SIZE(tests); ++i) {\
TestsHolder::instance().addTest(tests[i]);\
}\
}

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(*x))

struct TestAdder {
    TestsHolder::TestFunction f;
    
    TestAdder(TestsHolder::TestFunction f) : f{f} {}
    ~TestAdder() {
        TestsHolder::instance().addTest(f);
    }
};

#define TEST \
    (TestsHolder::TestFunction)[](Core& core, bool& result)

#define CHECK_EQUAL(a, b, msg) {\
    auto a_ = (a);\
    auto b_ = (b);\
    if(a_ != b_) {\
        std::cout << "Test failed: " << msg << std::endl << "\t" << #a << " == " << a_ << " , " << #b << " == " << b_ << std::endl;\
        result = false;\
    }}

#define CHECK_TRUE(x, msg) \
if(!(x)) {\
std::cout << "Test failed: " << msg << std::endl;\
result = false;\
}

#endif
