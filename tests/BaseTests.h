//
//  CoreTests.h
//  dcpu
//
//  Created by Artur Mazurek on 23.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_BaseTests_h
#define dcpu_BaseTests_h

#include "TestsUtil.h"

TESTS_START(BaseTests)

TEST {
    meta.name = "Basics";
    
    REQUIRE_TRUE(true, "Are tests working");
    REQUIRE_EQUAL(1, 1, "Basic checks");
},

// initialisations

TEST {
    meta.name = "Registers zero-initialisation";
    
    REQUIRE_EQUAL(core.registers().A, 0, "Is register A properly initialised");
    REQUIRE_EQUAL(core.registers().B, 0, "Is register B properly initialised");
    REQUIRE_EQUAL(core.registers().C, 0, "Is register C properly initialised");
    REQUIRE_EQUAL(core.registers().X, 0, "Is register X properly initialised");
    REQUIRE_EQUAL(core.registers().Y, 0, "Is register Y properly initialised");
    REQUIRE_EQUAL(core.registers().Z, 0, "Is register Z properly initialised");
    REQUIRE_EQUAL(core.registers().I, 0, "Is register I properly initialised");
    REQUIRE_EQUAL(core.registers().J, 0, "Is register J properly initialised");
    
    REQUIRE_EQUAL(core.registers().PC, 0, "Is register PC properly initialised");
    REQUIRE_EQUAL(core.registers().SP, 0xffff, "Is register SP properly initialised");
    REQUIRE_EQUAL(core.registers().EX, 0, "Is register EX properly initialised");
    REQUIRE_EQUAL(core.registers().IA, 0, "Is register IA properly initialised");
},

TEST {
    meta.name = "Registers zero-initialisation";
    
    core.registers().A = 0;
    core.registers().B = 1;
    core.registers().C = 2;
    core.registers().X = 3;
    core.registers().Y = 4;
    core.registers().Z = 5;
    core.registers().I = 6;
    core.registers().J = 7;
    core.registers().PC = 8;
    core.registers().SP = 9;
    core.registers().EX = 10;
    core.registers().IA = 11;
    
    // there are 12 registers
    for(int i = 0; i < 12; ++i) {
        REQUIRE_EQUAL(core.registers().reg(i), i, "Is register retrieved correctly");
    }
},

TEST {
    meta.name = "Memory zero-initailisation";
    
    for(uint16_t i = 0; i != 0xffff; ++i) {
        REQUIRE_EQUAL(core.memory(i), 0, "Is memory 0 initialised");
    }
},

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
}

TESTS_END

#endif
