//
//  CoreTests.h
//  dcpu
//
//  Created by Artur Mazurek on 09.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_CoreTests_h
#define dcpu_CoreTests_h

#include <chrono>

#include "TestsUtil.h"

TESTS_START(CoreTests)

TEST {
    meta.name = "Runnning core's own thread";
    
    Instruction i[] {
        { OP_STI, 0x00, 0x06 }, // STI A, I
        { OP_SET, 0x1c, 0x21 }, // SET PC, 0
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    int N = 5;
    std::chrono::milliseconds period{100};
    core.run(period);
    std::this_thread::sleep_for(5.5 * period);
    
    // this thread may sometimes fail due to scheduling, so need to always worry
    core.stop();
    
    CHECK_EQUAL(core.registers().A, N, "Is core running as it should in a thread");
}

TESTS_END

#endif
