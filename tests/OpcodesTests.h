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
    CHECK_EQUAL(core.registers().PC, 1, "Is program counter increasing 1"   );
    
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
        {OP_SET, 0x00, 0x11}, // SET A, [B + 4]
        {0x0004},             
        {OP_SET, 0x10, 0x20}, // SET [A + 5], 0xffff
        {0x0005},             
        {0x1010},             // 0x1010
    };
    
    core.setInstructions(i, ARRAY_SIZE(i));
    core.doCycle(2);
    CHECK_EQUAL(core.registers().A, 0x1010, "Can use [register + next word]");
    CHECK_EQUAL(core.registers().PC, 2, "PC should skip data");
    
    core.doCycle(2);
    CHECK_EQUAL(core.memory(0x1015), 0xffff, "Can address memory through [register + next word]");
    CHECK_EQUAL(core.registers().PC, 4, "PC should skip data");
},

TEST {
    meta.name = "PUSH & POP";
    
    Instruction i[] = {
        { OP_SET, 0x18, 0x22 }, // PUSH 1
        { OP_SET, 0x00, 0x18 }, // POP A
        { OP_SET, 0x18, 0x23 }, // PUSH 2
        { OP_SET, 0x18, 0x24 }, // PUSH 3
        { OP_SET, 0x00, 0x18 }, // POP A
        { OP_SET, 0x00, 0x18 }  // POP A
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle();
    CHECK_EQUAL(core.registers().SP, 0xffff, "Is stack pointer decreasing");
    CHECK_EQUAL(core.memory(0xffff), 1, "Are values pushed correctly");
    
    core.doCycle();
    CHECK_EQUAL(core.registers().SP, 0, "Is stack pointer increasing");
    CHECK_EQUAL(core.registers().A, 1, "Is value retrieved from stack");
    
    core.doCycle(3);
    CHECK_EQUAL(core.registers().A, 3, "Are multiple values pushed/popped correctly");
    
    core.doCycle();
    CHECK_EQUAL(core.registers().A, 2, "Are multiple values pushed/popped correctly");
},

TEST {
    meta.name = "PEEK & PICK";
    
    Instruction i[] = {
        { OP_SET, 0x18, 0x23 }, // PUSH 2
        { OP_SET, 0x00, 0x19 }, // SET A, [SP]
        { OP_SET, 0x18, 0x24 }, // PUSH 3
        { OP_SET, 0x18, 0x25 }, // PUSH 4
        { OP_SET, 0x18, 0x26 }, // PUSH 5
        { OP_SET, 0x18, 0x27 }, // PUSH 6
        { OP_SET, 0x18, 0x28 }, // PUSH 7
        { OP_SET, 0x00, 0x1a }, // SET A, [SP + 2]
        { 0x0002 },
        { OP_SET, 0x1a, 0x2a }, // SET [SP + 2], 9
        { 0x0002 }
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(2);
    CHECK_EQUAL(core.registers().A, 2, "Can you PEEK correctly?");
    
    core.doCycle(7);
    CHECK_EQUAL(core.registers().A, 5, "Can you PICK to register correctly?");
    
    core.doCycle(2);
    CHECK_EQUAL(core.memory(0xffff - 3), 9, "Can you PICK back to stack correctly?");
},

TEST {
    meta.name = "Taking next values";
    
    Instruction i[] = {
        { OP_SET, 0x00, 0x1f }, // SET A, 0xfafa
        { 0xfafa },
        { OP_SET, 0x1e, 0x1f }, // SET [0x0010], 0xabba
        { 0xabba },
        { 0x0010 }
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(5);
    
    CHECK_EQUAL(core.registers().A, 0xfafa, "Can set literal values to registers");
    CHECK_EQUAL(core.memory(0x0010), 0xabba, "Can set literal values to memory");
},

TEST {
    meta.name = "SP, PC, EX as values";
    
    Instruction i[] = {
        { OP_SET, 0x1b, 0x3f }, // SET SP, 30
        { OP_SET, 0x1c, 0x26 }, // SET PC, 5
        { 0xabba },
        { 0xabba },
        { 0xabba },
        { OP_SET, 0x1d, 0x20 }, // SET EX, 0xffff
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(3);
    CHECK_EQUAL(core.registers().SP, 30, "Can set stack pointer");
    CHECK_EQUAL(core.registers().PC, 6, "Can set program counter");
    CHECK_EQUAL(core.registers().EX, 0xffff, "Can set EX");
},

TEST {
    meta.name = "OP_ADD";
    
    Instruction i[] = {
        { OP_SET, 0x1d, 0x20 }, // SET EX, 0xffff
        { OP_SET, 0x00, 0x26 }, // SET A, 5
        { OP_ADD, 0x00, 0x3f }, // ADD A, 30
        
        { OP_ADD, 0x00, 0x1f }, // ADD A, 0xfffe
        { 0xfffe }
    };
    
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(4);
    CHECK_EQUAL(core.registers().A, 35, "Is adding correct");
    CHECK_EQUAL(core.registers().EX, 0, "Is EX set correctly when no overflow");
    
    core.doCycle(3);
    CHECK_EQUAL(core.registers().A, static_cast<uint16_t>(0xfffe + 35), "Does adding overflow correctly");
    CHECK_EQUAL(core.registers().EX, 1, "Is EX set correctly after overflow");
},

TEST {
    meta.name = "OP_SUB";
    
    Instruction i[] = {
        {OP_SET, 0x1d, 0x20}, // SET EX, 0xffff
        {OP_SET, 0x00, 0x1f}, // SET A, 0x001f
        { 0x001f },
        {OP_SUB, 0x00, 0x1f}, // SUB A, 0x0f
        { 0x000f },
        
        {OP_SUB, 0x00, 0x1f}, // SUB A, 0x20
        { 0x0020 }
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(6);
    CHECK_EQUAL(core.registers().A, 0x10, "Can subtract stuff");
    CHECK_EQUAL(core.registers().EX, 0, "Is EX set correctly when there's no overflow");
    
    core.doCycle(3);
    uint16_t temp = 0x10;
    temp -= 0x20;
    CHECK_EQUAL(core.registers().A, temp, "Can subtract stuff with overflow");
    CHECK_EQUAL(core.registers().EX, 0xffff, "Is EX set correctly when there's an overflow");
},

TEST {
    meta.name = "OP_MUL";
    
    Instruction i[] = {
        {OP_SET, 0x1d, 0x20}, // SET EX, 0xffff
        {OP_SET, 0x00, 0x1f}, // SET A, 0x20
        { 0x0020 },
        {OP_MUL, 0x00, 0x1f}, // MUL A, 0x20
        { 0x0020 },
        
        {OP_SET, 0x1e, 0x20}, // SET [0x8000], 0xffff
        { 0x8000 },
        
        {OP_MUL, 0x1e, 0x1f}, // MUL [0x8000], 0xffff
        { 0xffff },
        { 0x8000 }
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(6);
    CHECK_EQUAL(core.registers().A, 0x20 * 0x20, "Is multiplying correct");
    CHECK_EQUAL(core.registers().EX, 0, "Is EX set correctly after multiplying");
    
    core.doCycle(2);
    CHECK_EQUAL(core.memory(0x8000), 0xffff, "Is memory set correctly before multiplying");
    
    core.doCycle(4);
    uint32_t temp = 0xffff * 0xffff;
    CHECK_EQUAL(core.memory(0x8000), temp & 0xffff, "Is multiplying working correctly with big numbers");
    CHECK_EQUAL(core.registers().EX, temp >> 16, "Is EX set correctly after multiply overflow");
},

TEST {
    meta.name = "OP_MLI";
    
    Instruction i[] = {
        {OP_SET, 0x1d, 0x22}, // SET EX, 1
        {OP_SET, 0x00, 0x1f}, // SET A, -10
        { -10 },
        {OP_MLI, 0x00, 0x23}, // MLI A, 2
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(5);
    CHECK_EQUAL(static_cast<int16_t>(core.registers().A), -20, "Is multiply with sign working correctly");
    CHECK_EQUAL(static_cast<int16_t>(core.registers().EX), (-20) >> 16, "Is EX filled correctly");
},

TEST {
    meta.name = "OP_DIV";
    
    Instruction i[] = {
        { OP_SET, 0x1d, 0x22 }, // SET EX, 1
        { OP_SET, 0x00, 0x1f }, // SET A, 100
        { 100 },
        { OP_DIV, 0x00, 0x2b }, // DIV A, 10
        
        { OP_DIV, 0x01, 0x02 }, // DIV B, C
        
        { OP_SET, 0x00, 0x22 }, // SET A, 1
        { OP_DIV, 0x00, 0x23 }, // DIV A, 2
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(6);
    CHECK_EQUAL(core.registers().A, 10, "Division works");
    CHECK_EQUAL(core.registers().EX, 0, "Is EX set correctly after division");
    
    core.doCycle(3);
    CHECK_EQUAL(core.registers().C, 0, "Is division by 0 handled correctly");
    CHECK_EQUAL(core.registers().EX, 0, "Is division by 0 handled correctly");
    
    core.doCycle(4);
    CHECK_EQUAL(core.registers().A, 0, "Is division 1/2 handled correctly");
    CHECK_EQUAL(core.registers().EX, 0x10000/2, "Is division 1/2 handled correctly");
},

TEST {
    meta.name = "OP_DVI";
    
    Instruction i[] = {
        { OP_SET, 0x00, 0x1f }, // SET A, -248
        { -248 },
        { OP_DVI, 0x00, 0x1f }, // DVI A, 16 // == -15.5
        { 16 }
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(6);
    CHECK_EQUAL(static_cast<int16_t>(core.registers().A), -15, "Is pre-decimal part handled correctly");
    CHECK_EQUAL(core.registers().EX, 0x10000/2, "Is decimal part handled correctly");
},

TEST {
    meta.name = "OP_MOD";
    
    Instruction i[] = {
        { OP_SET, 0x00, 0x1f }, // SET A, 125
        { 125 },
        { OP_SET, 0x01, 0x1f }, // SET B, 2048
        { 2048 },
        { OP_SET, 0x02, 0x1f }, // SET C, 123
        { 123 },
        { OP_MOD, 0x00, 0x1f }, // MOD A, 120
        { 120 },
        { OP_MOD, 0x01, 0x23 },    // MOD B, 2
        { OP_MOD, 0x02, 0x21 }  // MOB C, 0
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(16);
    CHECK_EQUAL(core.registers().A, 5, "Is modulo working 1");
    CHECK_EQUAL(core.registers().B, 0, "Is modulo working 2");
    CHECK_EQUAL(core.registers().C, 0, "Is modulo by 0 working");
},

TEST {
    meta.name = "OP_MDI";
    
    Instruction i[] = {
        { OP_SET, 0x00, 0x1f }, // SET A, -7
        { -7 },
        { OP_SET, 0x01, 0x1f }, // SET B, -12
        { -12 },
        { OP_MDI, 0x00, 0x1f }, // MDI A, 16
        { 16 },
        { OP_MDI, 0x01, 0x21 }  // MDI B, 0
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(11);
    CHECK_EQUAL(static_cast<int16_t>(core.registers().A), -7, "Is signed modulo working");
    CHECK_EQUAL(core.registers().B, 0, "Is signed modulo by 0 working");
},

TEST {
    meta.name = "Logical operations - OR, AND, XOR";
    
    Instruction i[] = {
        { OP_SET, 0x00, 0x1f }, // SET A, 0xaaaa
        { 0xaaaa },
        { OP_SET, 0x01, 0x00 }, // SET B, A
        { OP_SET, 0x02, 0x00 }, // SET C, A
        { OP_AND, 0x00, 0x1f }, // AND A, 0x00ff
        { 0x00ff },
        { OP_BOR, 0x01, 0x1f }, // BOR B, 0x00ff
        { 0x00ff },
        { OP_XOR, 0x02, 0x1f }, // XOR C, 0x00ff
        { 0x00ff }
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(10);
    CHECK_EQUAL(core.registers().A, 0x00aa, "Is AND working correctly");
    CHECK_EQUAL(core.registers().B, 0xaaff, "Is BOR working correctly");
    CHECK_EQUAL(core.registers().C, 0xaa55, "Is XOR working correctly");
},

TEST {
    meta.name = "Shifting operators - SHR, ASR, SHL";
    
    Instruction i[] = {
        { OP_SET, 0x00, 0x26 }, // SET A, 5
        { OP_SHR, 0x00, 0x22 }, // SHR A, 1 -> 2
        
        { OP_SET, 0x00, 0x1f }, // SET A, -27
        { -0b11011 },
        { OP_ASR, 0x00, 0x22 }, // ASR A, 1 -> -14
        
        { OP_SET, 0x00, 0x1f }, // SET A, 0x8005
        { 0x8005 },
        { OP_SHL, 0x00, 0x22 }, // SHL A, 1 -> 10
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    core.doCycle(2);
    CHECK_EQUAL(core.registers().A, 2, "Is logical shift right working");
    CHECK_EQUAL(core.registers().EX, 0x8000, "Is bit move handled correctly");
    
    core.doCycle(3);
    CHECK_EQUAL(static_cast<int16_t>(core.registers().A), -14, "Is arithmetic shift right working correctly");
    CHECK_EQUAL(core.registers().EX, 0x8000, "Is bit move handled correctly");
    
    core.doCycle(3);
    CHECK_EQUAL(core.registers().A, 10, "Is shift left working correctly");
    CHECK_EQUAL(core.registers().EX, 1, "Is shift left working correctly (EX)");
}

TESTS_END

#endif
