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
    
    class Fibonacci : public Hardware {
    public:
        bool called;
        
        Fibonacci() : Hardware{1, 1, 2}, called{false}, m_a{1}, m_b{1} {}
        
        virtual void doReceiveInterrupt(Core& from) override {
            called = true;
            
            CHECK_EQUAL(from.registers().B, next(), "Is the value sent correct");
            
            if(from.registers().B > 100) {
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
    auto fibonacci = std::make_shared<Fibonacci>();
    
    // every HWN register B will contain next fibonacci number
    std::stringstream s{ R"(
            set a, 1
            set b, 1
            hwi 0
            hwi 0
            loop:
            add a, b
            set c, a
            set a, b
            set b, c
            hwi 0
            set pc, loop
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

TESTS_END

#endif