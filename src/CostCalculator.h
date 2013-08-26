//
//  CostCalculator.h
//  dcpu
//
//  Created by Artur Mazurek on 26.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __dcpu__CostCalculator__
#define __dcpu__CostCalculator__

#include <cstdint>

#include "Opcodes.h"

class Instruction;

class CostCalculator {
public:
    static unsigned getNormalCost(Opcode o, uint8_t b, uint8_t a);
    static unsigned getSpecialCost(Opcode o, uint8_t a);
    
private:
    static unsigned argumentCost(uint8_t arg);
};

#endif /* defined(__dcpu__CostCalculator__) */
