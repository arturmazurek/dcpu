//
//  TestsHolder.cpp
//  dcpu
//
//  Created by Artur Mazurek on 23.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "TestsHolder.h"

#include <iostream>

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
        
        m_currentSucceeded = true;
        TestMeta meta;
        try {
            test(c, meta);
        } catch (TestException&) {
            std::cout << "Test \"" << (meta.name.size() ? meta.name : "<no-name>") << "\" failed and stopped testing." << std::endl;
            m_currentSucceeded = false;
        } catch (std::exception& e) {
            std::cout << "Test \"" << (meta.name.size() ? meta.name : "<no-name>") << "\" failed with exception: \"" << e.what() << "\"" << std::endl;
            m_currentSucceeded = false;
        }
        
        if(m_currentSucceeded) {
            ++successful;
        } else {
            std::cout << "Test \"" << (meta.name.size() ? meta.name : "<no-name>") << "\" failed." << std::endl;
            ++failed;
        }
    }
    
    std::cout << "Run " << (successful + failed) << " tests of " << m_tests.size() << ". " << successful << " passed." << std::endl;
}

void TestsHolder::failed() {
    m_currentSucceeded = false;
}

void TestsHolder::addTest(TestFunction t) {
    m_tests.push_back(t);
}