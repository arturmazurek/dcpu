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

class Core;

class TestsHolder {
public:
    typedef std::function<void(Core&, bool&)> TestFunction;
    
    static TestsHolder& instance();
    
    void runTests();
    void addTest(TestFunction t);
    
private:
    TestsHolder() {}
    TestsHolder(const TestsHolder&);
    TestsHolder& operator=(const TestsHolder&);
    
private:
    std::list<TestFunction> m_tests;
};

#endif
