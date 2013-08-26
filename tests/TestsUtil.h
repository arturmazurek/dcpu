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

#define TEST \
    (TestsHolder::TestFunction)[](Core& core, bool& result, TestsHolder::TestMeta& meta)

#define DO_CHECK_EQUAL(a, b, msg, require) {\
    auto a_ = (a);\
    auto b_ = (b);\
    if(a_ != b_) {\
        std::cout << "Check failed: " << msg << std::endl << "\t" << #a << " == " << a_ << " , " << #b << " == " << b_ << std::endl;\
        result = false;\
        if(require) { \
            throw TestsHolder::TestException(msg + std::string("failed"));\
        }\
    }}

#define CHECK_EQUAL(a, b, msg) DO_CHECK_EQUAL(a, b, msg, false)

#define REQUIRE_EQUAL(a, b, msg) DO_CHECK_EQUAL(a, b, msg, true)

#define DO_CHECK_TRUE(x, msg, require) \
if(!(x)) {\
std::cout << "Check failed: " << msg << std::endl;\
result = false;\
if(require) { \
    throw TestsHolder::TestException(msg + std::string("failed"));\
}}

#define CHECK_TRUE(x, msg) DO_CHECK_TRUE(x, msg, false)

#define REQUIRE_TRUE(x, msg) DO_CHECK_TRUE(x, msg, true)

#endif
