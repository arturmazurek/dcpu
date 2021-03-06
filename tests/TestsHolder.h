//
//  TestsHolder.h
//  dcpu
//
//  Created by Artur Mazurek on 23.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_TestsHolder_h
#define dcpu_TestsHolder_h

#include <functional>
#include <list>
#include <string>
#include <stdexcept>

#include "Core.h"

class TestsHolder {
public:
    struct TestMeta {
        std::string name;
    };
    
    class TestException : std::runtime_error {
    public:
        TestException(const std::string& why) : std::runtime_error(why) {}
    };
    
    typedef std::function<void(Core&, TestMeta&)> TestFunction;
    
    static TestsHolder& instance();
    
    void runTests();
    void addTest(TestFunction t);
    
    void failed();
    
private:
    TestsHolder() {}
    TestsHolder(const TestsHolder&);
    TestsHolder& operator=(const TestsHolder&);
    
private:
    std::list<TestFunction> m_tests;
    bool m_currentSucceeded;
};

#endif
