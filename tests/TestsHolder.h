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
    static TestsHolder& instance();
    
    void runTests();
    void addTest(std::function<bool(Core&)> t);
    
private:
    TestsHolder() {}
    TestsHolder(const TestsHolder&);
    TestsHolder& operator=(const TestsHolder&);
    
private:
    std::list<std::function<bool(Core&)>> m_tests;
};

#endif
