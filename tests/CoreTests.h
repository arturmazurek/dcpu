//
//  CoreTests.h
//  dcpu
//
//  Created by Artur Mazurek on 23.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_CoreTests_h
#define dcpu_CoreTests_h

#include <functional>
#include <iostream>

#include "TestsHolder.h"

struct TestAdder {
    std::function<bool(Core&)> f;
    
    TestAdder(std::function<bool(Core&)> f) : f{f} {}
    ~TestAdder() {
        TestsHolder::instance().addTest(f);
    }
};

#define D_TEST(l) \
    TestAdder t##l = (std::function<bool(Core&)>)[](Core& core)

#define TEST D_TEST(__LINE__)

#define CHECK_TRUE(x, msg) \
    if(!(x)) {\
        std::cout << "Test failed: " << msg << std::endl;\
        return false;\
    }

static void prepareCoreTests() {
    using namespace std;
    
    TEST {
        CHECK_TRUE(true, "Are tests working");
        return true;
    };
    
}

#endif
