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

#include "Registers.h"

class Core {
public:
    Core();
    
    void resetState();
    
private:
    static const int MEMORY_SIZE = 0x10000;
    static const int STACK_START = 0xffff;
    std::array<uint16_t, MEMORY_SIZE> m_memory;
    
    Registers m_registers;
};

#endif /* defined(__dcpu__Core__) */
