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

#include <iostream>

#define ADD_TESTS(name) prepare##name()

#define TESTS_START(name) void prepare##name() {\
TestsHolder::TestFunction tests[] = {

#define TESTS_END };\
for(int i = 0; i < ARRAY_SIZE(tests); ++i) {\
TestsHolder::instance().addTest(tests[i]);\
}\
}

#define TEST \
    (TestsHolder::TestFunction)[](Core& core, TestsHolder::TestMeta& meta)

template <typename aT, typename bT>
static inline void _doCheckEqual(const aT& a, const bT& b, const std::string& aMsg, const std::string& bMsg, const std::string& msg, bool require) {
    if(a != b) {
        std::cout << "Check failed: " << msg << std::endl << "\t" << aMsg << " == " << a << " , " << bMsg << " == " << b << std::endl;
        TestsHolder::instance().failed();
        if(require) {
            throw TestsHolder::TestException(msg + " failed");
        }
    }
}

#define DO_CHECK_EQUAL(a, b, msg, require) \
    _doCheckEqual(a, b, #a, #b, msg, require)

#define CHECK_EQUAL(a, b, msg) DO_CHECK_EQUAL(a, b, msg, false)

#define REQUIRE_EQUAL(a, b, msg) DO_CHECK_EQUAL(a, b, msg, true)

template <typename T>
static inline void _doCheckTrue(const T& x, const std::string& xMsg, const std::string& msg, bool require) {
    if(!x) {
        std::cout << "Check failed (\"" << xMsg << "\"):" << msg << std::endl;
        TestsHolder::instance().failed();
        if(require) {
            throw TestsHolder::TestException(msg + "failed");
        }
    }
}

#define DO_CHECK_TRUE(x, msg, require) \
    _doCheckTrue(x, #x, msg, require)

#define CHECK_TRUE(x, msg) DO_CHECK_TRUE(x, msg, false)

#define REQUIRE_TRUE(x, msg) DO_CHECK_TRUE(x, msg, true)

#endif
