//
//  AssemblerTests.h
//  dcpu
//
//  Created by Artur on 25/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_AssemblerTests_h
#define dcpu_AssemblerTests_h

#include <sstream>
#include <thread>
#include <utility>

#include "TestsUtil.h"

#include "ASMUtils.h"
#include "Assembler.h"
#include "Hardware.h"
#include "Lexer.h"

class Fibonacci : public Hardware {
public:
    bool called;
    
    Fibonacci() : Hardware{1, 1, 2}, called{false}, m_a{1}, m_b{1} {}
    
    virtual void doReceiveInterrupt(Core& from) override {
        called = true;
        
        CHECK_EQUAL(from.registers().A, next(), "Is the value sent correct");
        
        if(from.registers().A > 100) {
            from.stop();
        }
    }
    
private:
    int next() {
        auto res = m_a;
        
        m_a += m_b;
        std::swap(m_a, m_b);
        
        return res;
    }
    
    int m_a;
    int m_b;
};

TESTS_START(AssemblerTests)

TEST {
    meta.name = "Basic assembler";

    std::stringstream s{"set a, 1"};
    
    Assembler assembler;
    assembler.setLexer(std::make_unique<Lexer>(s));
    assembler.assemble();
    
    const auto& binary = assembler.binary();
    REQUIRE_EQUAL(binary.size(), 1, "Is binary assembled");
    
    Instruction i = { OP_SET, 0x00, 0x22 }; /// SET A, 1
    uint16_t val = *reinterpret_cast<uint16_t*>(i.raw);
    CHECK_EQUAL(binary[0], val, "Is assembled correctly");
    
    core.setMemory(binary);
    core.cycle();
    
    CHECK_EQUAL(core.registers().A, 1, "Is register updated correctly");
},

TEST {
    meta.name = "More complicated assembler";
    
    std::stringstream s{ R"(
            set a, 1
            add a, 124
        )"
    };
    
    Assembler assembler;
    assembler.setLexer(std::make_unique<Lexer>(s));
    assembler.assemble();
    
    core.setMemory(assembler.binary());
    core.cycle(1);
    CHECK_EQUAL(core.registers().A, 1, "Is A set correctly");

    core.cycle(3);
    CHECK_EQUAL(core.registers().A, 125, "Is addition correct");
},

TEST {
    meta.name = "More complicated assembler 2";
    
    auto fibonacci = std::make_shared<Fibonacci>();
    
    // every HWN register B will contain next fibonacci number
    std::stringstream s{ R"(
            set a, 1
            set b, 1
        loop:
            hwi 0           ; register a is checked by the attached hardware
            add a, b        ; a += b, swap(a, b)
            set c, a
            set a, b
            set b, c
            set pc, loop    ; jmp loop
        )"
    };
    
    Assembler a;
    a.setLexer(std::make_unique<Lexer>(s));
    a.assemble();
    
    core.setMemory(a.binary());
    core.attachHardware(fibonacci);
    core.run();
    core.join();
    
    REQUIRE_TRUE(fibonacci->called, "Was the fibonacci counter actually called");
},

TEST {
    meta.name = "More complicated assembler 2 with jmp";
    
    auto fibonacci = std::make_shared<Fibonacci>();
    
    // every HWN register B will contain next fibonacci number
    std::stringstream s{ R"(
            set a, 1
            set b, 1
        loop:
            hwi 0           ; register a is checked by the attached hardware
            add a, b        ; a += b, swap(a, b)
            set c, a
            set a, b
            set b, c
            jmp loop
        )"
    };
    
    Assembler a;
    a.setLexer(std::make_unique<Lexer>(s));
    a.assemble();
    
    core.setMemory(a.binary());
    core.attachHardware(fibonacci);
    core.run();
    core.join();
    
    REQUIRE_TRUE(fibonacci->called, "Was the fibonacci counter actually called");
},
    
TEST {
    meta.name = "Reserve word - resw";
    
    std::stringstream s{"resw 2"};
    Instruction i[] = { { 0 }, { 0 } };
    
    Assembler a;
    a.setLexer(std::make_unique<Lexer>(s));
    a.assemble();
    
    REQUIRE_EQUAL(a.binary().size(), ARRAY_SIZE(i), "Is resw reserving proper amount");
    CHECK_EQUAL(memcmp(a.binary().data(), i, ARRAY_SIZE(i)), 0, "Is resw actually reserving stuff?");
},
    
TEST {
    meta.name = "RESW 2";
    
    std::stringstream s{ R"(
            set pc, 5
            resw 4
            set a, 42
        )"
    };
    
    Assembler a;
    a.setLexer(std::make_unique<Lexer>(s));
    a.assemble();
    
    core.setMemory(a.binary());
    core.cycle(3);
    
    CHECK_EQUAL(core.registers().A, 42, "Was a set correctly");
},
    
TEST {
    meta.name = "RESW + labels";
    
    std::stringstream s{ R"(
            set a, 1
            set a, 2
            set a, 3
            set a, 4
        reserve: resw 32-reserve
        )"
    };
    
    Assembler a;
    a.setLexer(std::make_unique<Lexer>(s));
    a.assemble();
    
    CHECK_EQUAL(a.binary().size(), 32, "Is proper size reserved");
},
    
TEST {
    meta.name = "DW + labels";
    
    std::stringstream s{ "dw 1,2,3,4,5" };
    Instruction i[] = {
        {1}, {2}, {3}, {4}, {5}
    };
    
    Assembler a;
    a.setLexer(std::make_unique<Lexer>(s));
    a.assemble();
    
    REQUIRE_EQUAL(a.binary().size(), ARRAY_SIZE(i), "Are values properly declared");
    CHECK_EQUAL(memcmp(a.binary().data(), i, ARRAY_SIZE(i)), 0, "Are values properly declared 2");
},

TESTS_END

#endif
