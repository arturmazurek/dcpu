//
//  CoreTests.h
//  dcpu
//
//  Created by Artur Mazurek on 23.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_BaseTests_h
#define dcpu_BaseTests_h

#include "Core.h"
#include "Instruction.h"
#include "TestsUtil.h"

TESTS_START(BaseTests)

TEST {
    meta.name = "Basics";
    
    REQUIRE_TRUE(true, "Are tests working");
    REQUIRE_EQUAL(1, 1, "Basic checks");
},

TEST {
    meta.name = "Endiannes handling";
    
    Instruction i = {0x00ff};
    REQUIRE_EQUAL(i.raw[0], 0xff, "Is endianness handled correctly");
    REQUIRE_EQUAL(i.raw[1], 0x00, "Is endianness handled correctly");
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
    REQUIRE_EQUAL(core.registers().SP, 0, "Is register SP properly initialised");
    REQUIRE_EQUAL(core.registers().EX, 0, "Is register EX properly initialised");
    REQUIRE_EQUAL(core.registers().IA, 0, "Is register IA properly initialised");
},

TEST {
    meta.name = "Registers retrieval";
    
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
    
    REQUIRE_EQUAL(core.registers().value(REG_A), 0, "Is register A retrieved correctly");
    REQUIRE_EQUAL(core.registers().value(REG_B), 1, "Is register B retrieved correctly");
    REQUIRE_EQUAL(core.registers().value(REG_C), 2, "Is register C retrieved correctly");
    REQUIRE_EQUAL(core.registers().value(REG_X), 3, "Is register X retrieved correctly");
    REQUIRE_EQUAL(core.registers().value(REG_Y), 4, "Is register Y retrieved correctly");
    REQUIRE_EQUAL(core.registers().value(REG_Z), 5, "Is register Z retrieved correctly");
    REQUIRE_EQUAL(core.registers().value(REG_I), 6, "Is register I retrieved correctly");
    REQUIRE_EQUAL(core.registers().value(REG_J), 7, "Is register J retrieved correctly");
    REQUIRE_EQUAL(core.registers().value(REG_PC), 8, "Is register PC retrieved correctly");
    REQUIRE_EQUAL(core.registers().value(REG_SP), 9, "Is register SP retrieved correctly");
    REQUIRE_EQUAL(core.registers().value(REG_EX), 10, "Is register EX retrieved correctly");
},

TEST {
    meta.name = "Memory zero-initailisation";
    
    for(uint16_t i = 0; i != 0xffff; ++i) {
        REQUIRE_EQUAL(core.memory(i), 0, "Is memory 0 initialised");
    }
}

TESTS_END

#endif
