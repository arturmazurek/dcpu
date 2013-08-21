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

#include "Instruction.h"
#include "Registers.h"

class Core {
public:
    Core();
    
    void resetState();
    void setMemory(uint16_t* m, unsigned size, unsigned startingAt = 0);
    void setInstructions(Instruction* m, unsigned size, unsigned startingAt = 0);
    
    void doCycle();
    
    void printRegisters() const;
    
private:
    bool interruptsEnabled() const;
    bool handleInterrupt();
    
    void fetch();
    void decode();
    void execute();
    
    void executeNormal();
    void executeSpecial();
    
private:
    static const int MEMORY_SIZE = 0x10000;
    static const int STACK_START = 0xffff;
    std::array<uint16_t, MEMORY_SIZE> m_memory;
    
    Instruction m_current;
    struct Decoded {
        int costLeft;
        uint8_t element1;
        uint8_t element2;
        uint8_t element3;
    } m_decoded;
    
    Registers m_registers;
};

#endif /* defined(__dcpu__Core__) */
