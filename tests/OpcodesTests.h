//
//  OpcodesTests.h
//  dcpu
//
//  Created by Artur Mazurek on 24.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_OpcodesTests_h
#define dcpu_OpcodesTests_h

#include "TestsUtil.h"

TESTS_START(OpcodesTests)

TEST {
    meta.name = "OP_SET, PC & parameters 1";
    
    Instruction i[] = {
        {OP_SET, 0x00, 0x20}, // SET A, 0xffff
        {OP_SET, 0x09, 0x20}, // SET [B], 0xffff
        {OP_SET, 0x02, 0x00}, // SET C, A
        {OP_SET, 0x03, 0x0c}, // SET X, [Y]
        {OP_SET, 0x00, 0x3f}, // SET A, 30
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle();
    CHECK_EQUAL(core.registers().A, 0xffff, "Is A set properly");
    CHECK_EQUAL(core.registers().PC, 1, "Is program counter increasing 1");
    
    core.doCycle();
    CHECK_EQUAL(core.memory(0), 0xffff, "Is memory register-addressed correctly");
    CHECK_EQUAL(core.registers().PC, 2, "Is program counter increasing 2");
    
    core.doCycle();
    CHECK_EQUAL(core.registers().C, 0xffff, "Can assign register to register");
    CHECK_EQUAL(core.registers().PC, 3, "Is program counter increasing 3");
    
    core.doCycle();
    CHECK_EQUAL(core.registers().X, 0xffff, "Can assing registers from memory");
    CHECK_EQUAL(core.registers().PC, 4, "Is program counter increasing 3");
    
    core.doCycle();
    CHECK_EQUAL(core.registers().A, 30, "Can assign register with different value");
    CHECK_EQUAL(core.registers().PC, 5, "Is program counter increasing 3");
},

TEST {
    meta.name = "OP_SET & [register + next word]";
    
    Instruction i[] = {
        {OP_SET, 0x00, 0x11},
        {0x0002},
        {0x1010}
    };
    
    core.setInstructions(i, ARRAY_SIZE(i));
    core.doCycle();
    CHECK_EQUAL(core.registers().A, 0x1010, "Can use [register + next word]");
}

TESTS_END

#endif
