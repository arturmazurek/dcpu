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
},

TEST {
    meta.name = "INT - software interrupt with interrupts disabled";
    
    Instruction i[] = {
        { OP_SET, 0x00, 0x21 }, // SET A, 0
        { OP_SET, 0x00, 0x22 }, // SET A, 1
        { OP_INT, 0x20 },       // INT 0xffff
        { OP_SET, 0x00, 0x23 }, // SET A, 2
        { OP_SET, 0x00, 0x24 }, // SET A, 3
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(8);
    CHECK_EQUAL(core.registers().A, 3, "Is execution happening as normal with interrupts disabled");
},

TEST {
    meta.name = "INT - software interrupt with interrupts enabled";
    
    Instruction i[] = {
        { OP_IAS, 0x2b },       // IAS 10
        { OP_INT, 0x20 },       // INT 0xffff
        { OP_SET, 0x01, 0x26 }, // SET A, 5
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { OP_SET, 0x02, 0x26 }, // SET B, 5
        { OP_RFI, 0x22 },       // RFI
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle();
    CHECK_EQUAL(core.registers().IA, 10, "Is IAS working correctly");
    
    core.doCycle(4);
    CHECK_EQUAL(core.registers().A, 0xffff, "Is A set correctly to interrupt message");
    CHECK_EQUAL(core.registers().PC, 10, "Is PC set correctly in interrupt");
}

TESTS_END

#endif
