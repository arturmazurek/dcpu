//
//  CoreTests.h
//  dcpu
//
//  Created by Artur Mazurek on 23.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_CoreTests_h
#define dcpu_CoreTests_h

#include "TestsUtil.h"

TESTS_START(CoreTests)

TEST {
    CHECK_TRUE(true, "Are tests working");
    CHECK_EQUAL(1, 1, "Basic checks");
},

// initialisations

TEST {
    CHECK_EQUAL(core.registers().A, 0, "Is register A properly initialised");
    CHECK_EQUAL(core.registers().B, 0, "Is register B properly initialised");
    CHECK_EQUAL(core.registers().C, 0, "Is register C properly initialised");
    CHECK_EQUAL(core.registers().X, 0, "Is register X properly initialised");
    CHECK_EQUAL(core.registers().Y, 0, "Is register Y properly initialised");
    CHECK_EQUAL(core.registers().Z, 0, "Is register Z properly initialised");
    CHECK_EQUAL(core.registers().I, 0, "Is register I properly initialised");
    CHECK_EQUAL(core.registers().J, 0, "Is register J properly initialised");
    
    CHECK_EQUAL(core.registers().PC, 0, "Is register PC properly initialised");
    CHECK_EQUAL(core.registers().SP, 0xffff, "Is register SP properly initialised");
    CHECK_EQUAL(core.registers().EX, 0, "Is register EX properly initialised");
    CHECK_EQUAL(core.registers().IA, 0, "Is register IA properly initialised");
},

TEST {
    for(uint16_t i = 0; i != 0xffff; ++i) {
        bool zero = core.memory(i) == 0;
        CHECK_TRUE(zero, "Is memory 0 initialised");
        if(!zero) {
            break;
        }
    }
},

// basic opcodes
TEST {
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
    
    core.doCycle();
    CHECK_EQUAL(core.registers().A, 30, "Can assign register with different value");
}

TESTS_END

#endif
