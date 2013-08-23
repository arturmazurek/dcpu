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
},

TEST {
    CHECK_TRUE(core.registers().A == 0, "Is register A properly initialised");
    CHECK_TRUE(core.registers().B == 0, "Is register B properly initialised");
    CHECK_TRUE(core.registers().C == 0, "Is register C properly initialised");
    CHECK_TRUE(core.registers().D == 0, "Is register D properly initialised");
    CHECK_TRUE(core.registers().X == 0, "Is register X properly initialised");
    CHECK_TRUE(core.registers().Y == 0, "Is register Y properly initialised");
    CHECK_TRUE(core.registers().Z == 0, "Is register Z properly initialised");
    CHECK_TRUE(core.registers().I == 0, "Is register I properly initialised");
    CHECK_TRUE(core.registers().J == 0, "Is register J properly initialised");
    
    CHECK_TRUE(core.registers().PC == 0, "Is register PC properly initialised");
    CHECK_TRUE(core.registers().SP == 0xffff, "Is register SP properly initialised");
    CHECK_TRUE(core.registers().EX == 0, "Is register EX properly initialised");
    CHECK_TRUE(core.registers().IA == 0, "Is register IA properly initialised");
},

TEST {
    for(uint16_t i = 0; i != 0xffff; ++i) {
        bool zero = core.memory(i) == 0;
        CHECK_TRUE(zero, "Is memory 0 initialised");
        if(!zero) {
            break;
        }
    }
}

TESTS_END

#endif
