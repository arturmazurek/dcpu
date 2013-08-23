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

#define TEST t = [](Core& core)

static void prepareCoreTests() {
    using namespace std;
    
    TestsHolder& th = TestsHolder::instance();
    
    th.addTest([](Core& core) {
        cout << "Tests working" << endl;
        return true;
    });
    
    th.addTest([](Core& core) {
        cout << "Yes, certainly working" << endl;
        return true;
    });
    
}

#endif
