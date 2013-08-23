//
//  TestsHolder.cpp
//  dcpu
//
//  Created by Artur Mazurek on 23.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "TestsHolder.h"

#include <iostream>

#include "Core.h"

TestsHolder& TestsHolder::instance() {
    static TestsHolder inst;
    return inst;
}

void TestsHolder::runTests() {
    int successful = 0;
    int failed = 0;
    
    for(auto& test : m_tests) {
        Core c;
        c.resetState();
        
        bool s = true;
        test(c, s);
        
        if(s) {
            ++successful;
        } else {
            ++failed;
        }
    }
    
    std::cout << "Run " << (successful + failed) << " tests. " << successful << " passed." << std::endl;
}

void TestsHolder::addTest(TestFunction t) {
    m_tests.push_back(t);
}