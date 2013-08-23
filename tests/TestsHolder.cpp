//
//  TestsHolder.cpp
//  dcpu
//
//  Created by Artur Mazurek on 23.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "TestsHolder.h"

#include "Core.h"

TestsHolder& TestsHolder::instance() {
    static TestsHolder inst;
    return inst;
}

void TestsHolder::runTests() {
    for(auto& test : m_tests) {
        Core c;
        c.resetState();
        
        test(c);
    }
}

void TestsHolder::addTest(std::function<bool(Core&)> t) {
    m_tests.push_back(t);
}