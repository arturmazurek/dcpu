//
//  SpecialOpcodesTests.h
//  dcpu
//
//  Created by Artur Mazurek on 27/08/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_SpecialOpcodesTests_h
#define dcpu_SpecialOpcodesTests_h

#include "TestsUtil.h"

TESTS_START(SpecialOpcodesTests)

TEST {
    meta.name = "JSR test";
    
    Instruction i[] = {
        { OP_JSR, 0x26 },       // JSR 5
        { OP_SET, 0x00, 0x28 }, // SET A, 7
        { 0 },
        { 0 },
        { 0 },
        { OP_SET, 0x00, 0x22 }, // SET A, 1
        { OP_JSR, 0x22 },       // JSR 1
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(4);
    CHECK_EQUAL(core.registers().A, 1, "Is JSR working");
    CHECK_EQUAL(core.registers().PC, 6, "Is JSR working");
    CHECK_EQUAL(core.registers().SP, 0xffff, "Is SP correctly updated");
    CHECK_EQUAL(core.memory(0xffff), 1, "Is next instruction's address correctly pushed");
    
    core.doCycle(4);
    CHECK_EQUAL(core.registers().A, 7, "Is JSR working 2");
    CHECK_EQUAL(core.registers().PC, 2, "Is JSR working 2");
    CHECK_EQUAL(core.registers().SP, 0xfffe, "Is SP correctly updated 2");
    CHECK_EQUAL(core.memory(0xfffe), 7, "Is next instruction's address correctly pushed 2");
}

TESTS_END

#endif
