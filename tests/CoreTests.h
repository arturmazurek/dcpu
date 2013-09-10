//
//  CoreTests.h
//  dcpu
//
//  Created by Artur Mazurek on 09.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_CoreTests_h
#define dcpu_CoreTests_h

#include <chrono>
#include <thread>

#include "Hardware.h"
#include "TestsUtil.h"

TESTS_START(CoreTests)

TEST {
    meta.name = "Runnning core's own thread";
    
    Instruction i[] {
        { OP_STI, 0x00, 0x06 }, // STI A, I
        { OP_SET, 0x1c, 0x21 }, // SET PC, 0
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    
    const int N = 5;
    std::chrono::milliseconds period{100}; 
    core.run(period / 3); // 3 because it takes 3 cycles to do the instructions
    std::this_thread::sleep_for(5.5 * period);
    
    // this thread may sometimes fail due to scheduling, so need to always worry
    core.stop();
    
    CHECK_EQUAL(core.registers().A, N, "Is core running as it should in a thread");
},

TEST {
    meta.name = "Running sending interrupts";
    
    class InterruptCounter : public Hardware {
    public:
        std::atomic<unsigned> counter;
        std::condition_variable cv;
        
        InterruptCounter() : Hardware{1, 1, 1}, counter{0} {}
        
        void doReceiveInterrupt(Core& from) override {
            ++counter;
            cv.notify_one();
        }
    };
    std::shared_ptr<InterruptCounter> counter{new InterruptCounter{}};
    
    Instruction i[] = {
        { OP_HWI, 0x21 },       // HWI 0
        { OP_SET, 0x1c, 0x21 }, // SET PC 0
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    core.attachHardware(counter);
    
    const int N = 20;
    core.run();
    
    std::mutex m;
    std::unique_lock<decltype(m)> lock{m};
    counter->cv.wait(lock, [&]() {
        return counter->counter > N;
    });

    core.stop();
    
    CHECK_TRUE(counter->counter.load() > N, "Are interrupts called in a core's runloop?");
},

TEST {
    meta.name = "Running core receiving interrupts";
    
    class Ender : public Hardware {
    public:
        bool ended;
        std::condition_variable cv;
        
        Ender() : Hardware{1, 1, 1}, ended{false} {}
        
        void doReceiveInterrupt(Core& from) override {
            ended = true;
            cv.notify_one();
        }
    };
    std::shared_ptr<Ender> ender{new Ender{}};
    
    static constexpr uint16_t N = 20;
    
    Instruction i[] = {
        { OP_IAS, 0x26 },               // IAS 5
        { OP_IFE, 0x01, 0x21 + (N-1) }, // IFE B, (20 - 1)
        { OP_HWI, 0x21 },               // HWI 0
        { OP_SET, 0x1c, 0x22 },         // SET PC, 1
        { 0 },
        { OP_STI, 0x01, 0x06 },         // STI B, I
        { OP_RFI, 0x00 }                // RFI
    };
    core.setInstructions(i, ARRAY_SIZE(i));
    core.attachHardware(ender);
    
    core.run();
    for(uint16_t i = 0; i < N; ++i) {
        core.receiveInterrupt(i);
    }

    std::mutex m;
    std::unique_lock<decltype(m)> lock{m};
    ender->cv.wait(lock, [&]() {
        return ender->ended;
    });

    core.stop();

    CHECK_EQUAL(core.registers().I, N, "Were core's registers properly updated in the loop");
    CHECK_EQUAL(core.registers().B, N-1, "Were core's registers properly updated in the loop 2");
}

TESTS_END

#endif
