    //
//  SpecialOpcodesTests.h
//  dcpu
//
//  Created by Artur Mazurek on 27/08/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_SpecialOpcodesTests_h
#define dcpu_SpecialOpcodesTests_h

#include <memory>

#include "GenericClock.h"
#include "Hardware.h"
#include "Manufacturers.h"
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
    meta.name = "Interrupts 1";
    
    Instruction i[] = {
        { OP_IAS, 0x2b },       // IAS 10
        { OP_INT, 0x20 },       // INT 0xffff
        { OP_SET, 0x00, 0x26 }, // SET A, 5
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { OP_SET, 0x01, 0x26 }, // SET B, 5
        { OP_RFI, 0x22 },       // RFI
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle();
    CHECK_EQUAL(core.registers().IA, 10, "Is IAS working correctly");
    
    core.doCycle(4);
    CHECK_EQUAL(core.registers().A, 0xffff, "Is A set correctly to interrupt message");
    CHECK_EQUAL(core.registers().PC, 10, "Is PC set correctly in interrupt");
    CHECK_EQUAL(core.registers().B, 0, "Is INT taking enough time");
    
    core.receiveInterrupt(0x1234);
    
    core.doCycle();
    CHECK_EQUAL(core.registers().B, 5, "Are interrupts disabled in interrupt handler");
    CHECK_EQUAL(core.registers().PC, 11, "Are interrupts correctly disabled in an interrupt handler");
    
    core.doCycle(3);
    CHECK_EQUAL(core.registers().PC, 10, "Can trigger interrupt after RFI");
    
    core.doCycle(4);
    CHECK_EQUAL(core.registers().PC, 2, "Is RFI working correctly");
    
    core.doCycle();
    CHECK_EQUAL(core.registers().A, 5, "Is later on the core working correctly");
},

TEST {
    meta.name = "Interrupts 2";
    
    Instruction i[] = {
        { OP_IAS, 0x25 },       // IAG, 4
        { OP_STI, 0x00, 0x06 }, // STI A, I
        { OP_SET, 0x1c, 0x22 }, // SET PC, 1
        { 0 },
        { OP_RFI, 0x00 }        // RFI
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.receiveInterrupt(0x1234);
    core.doCycle();
    CHECK_EQUAL(core.registers().PC, 4, "Is interrupt correctly handled after setting interrupt address");
    
    int N = 5;
    for(int i = 0; i < N; ++i) {
        core.doCycle(3); // RFI
        core.doCycle(2); // STI
        CHECK_EQUAL(core.registers().A, i, "Is register A being correctly updated");
        core.receiveInterrupt(0x1234);
        core.doCycle();  // SET & interrupt handled after ie
    }
},

TEST {
    meta.name = "HWN";
    
    Instruction i[] = {
        { OP_HWN, 0x00 }, // HWN A
        { OP_HWN, 0x00 }, // HWN A
        { OP_HWN, 0x00 }, // HWN A
        { OP_HWN, 0x00 }  // HWN A
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(2);
    CHECK_EQUAL(core.registers().A, 0, "No clocks attached in the beginning");
    
    std::shared_ptr<Hardware> c1{new GenericClock{}};
    core.attachHardware(c1);
    core.doCycle(2);
    CHECK_EQUAL(core.registers().A, 1, "Is first clock correctly counted");
    
    std::shared_ptr<Hardware> c2{new GenericClock{}};
    core.attachHardware(c2);
    core.doCycle(2);
    CHECK_EQUAL(core.registers().A, 2, "Is second clock correctly added");
    
    CHECK_TRUE(core.hasHardware(c1), "Does the core know it has clocks attached?");
    CHECK_TRUE(core.hasHardware(c2), "Does the core know it has clocks attached?");
    
    core.detachHardware(c1);
    core.doCycle(2);
    CHECK_EQUAL(core.registers().A, 1, "Is HWN working correctly after detach");
    
    CHECK_TRUE(core.hasHardware(c2), "Is clock 2 still attached");
},

TEST {
    meta.name = "Hardware query";
    
    Instruction i[] = {
        { OP_HWQ, 0x00 }
    };
    core.setInstructions(i, ARRAY_SIZE(i));

    std::shared_ptr<Hardware> c{new GenericClock{}};
    core.attachHardware(c);
    core.doCycle(4);
    
    CHECK_EQUAL(core.registers().A, GenericClock::HARDWARE_ID & 0xffff, "Is lower half of hardware id correct");
    CHECK_EQUAL(core.registers().B, GenericClock::HARDWARE_ID >> 16, "Is lower half of hardware id correct");
    CHECK_EQUAL(core.registers().C, GenericClock::VERSION, "Is version retrieved correctly");
    CHECK_EQUAL(core.registers().X, Manufacturers::NYA_ELEKTRISKA & 0xffff, "Is manufacturer's lower half correct");
    CHECK_EQUAL(core.registers().Y, Manufacturers::NYA_ELEKTRISKA >> 16, "Is manufacturer's higher half correct");
},

TEST {
    meta.name = "Sending interrupts to hardware";
    
    class TestHardware : public Hardware {
    public:
        bool received;
        
        TestHardware() : Hardware{1, 1, 1}, received{false} {}
        
        void doReceiveInterrupt(Core& from) {
            received = true;
            from.registers().A = 16;
        }
    };
    
    Instruction i[] = {
        { OP_HWI, 0x21 } // HWI 0
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    std::shared_ptr<TestHardware> h{new TestHardware{}};
    core.attachHardware(h);
    core.doCycle(4);
    
    CHECK_TRUE(h->received, "Are hardware interrupts sent to hardware");
    CHECK_EQUAL(core.registers().A, 16, "Is the processor correctly updated");
}

TESTS_END

#endif
