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

#include "TestsUtil.h"

static void prepareCoreTests() {
    using namespace std;
    
    TEST {
        CHECK_TRUE(true, "Are tests working");
    };
    
}

#endif
