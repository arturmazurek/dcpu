//
//  Core.h
//  dcpu
//
//  Created by Artur Mazurek on 20.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __dcpu__Core__
#define __dcpu__Core__

#include <cstdint>

#include <array>
#include <queue>
#include <thread>

#include "Instruction.h"
#include "Registers.h"

class Core {
public:    
    Core();
    
    void resetState();
    void setMemory(uint16_t* m, unsigned size, unsigned startingAt = 0);
    void setInstructions(Instruction* m, unsigned size, unsigned startingAt = 0);
    
    void doCycle(unsigned cycles = 1);
    
    void printRegisters() const;
    void printMemory(uint16_t start, uint16_t end) const;
    
    const Registers& registers() const;
    Registers& registers();
    
    uint16_t memory(uint16_t at) const;
    
    void sendInterrupt(uint16_t message);
    
private:
    bool interruptsEnabled() const;
    
    bool queueInterrupts() const;
    void setQueueInterrupts(bool queueInterrupts);
    
    void handleInterrupt();
    
    void fetch();
    void decode();
    void execute();
    
    void executeNormal();
    void executeSpecial();
    
    uint16_t* checkOperand(uint16_t& operand, bool inA);
    
private:
    static const int MEMORY_SIZE = 0x10000;
    std::array<uint16_t, MEMORY_SIZE> m_memory;
    
    Instruction m_current;
    struct Decoded {
        int costLeft;
        bool special;
        
        Opcode opcode;
        uint16_t a;
        uint16_t b;
        
        uint16_t* source;
        uint16_t* target;
    } m_decoded;
    
    bool m_skipping;
    
    Registers m_registers;
    
    static const int MAX_INTERRUPTS = 256;
    mutable std::recursive_mutex m_interruptsMutex;
    bool m_queueInterrupts;
    std::queue<uint16_t> m_interruptsQueue;
};

#endif /* defined(__dcpu__Core__) */
