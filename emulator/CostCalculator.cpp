//
//  CostCalculator.cpp
//  dcpu
//
//  Created by Artur Mazurek on 26.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "CostCalculator.h"

#include <cassert>

static const int s_normalCosts[] {
    CostCalculator::INVALID_COST, // OP_NONE = 0x00, // special
    1, // OP_SET = 0x01, // sets b to a
    2, // OP_ADD = 0x02, // sets b to b+a, sets EX to 0x0001 if there's an overflow, 0x0 otherwise
    2, // OP_SUB = 0x03, // sets b to b-a, sets EX to 0xffff if there's an underflow, 0x0 otherwise
    2, // OP_MUL = 0x04, // sets b to b*a, sets EX to ((b*a)>>16)&0xffff (treats b, a as unsigned)
    2, // OP_MLI = 0x05, // like MUL, but treat b, a as signed
    3, // OP_DIV = 0x06, // sets b to b/a, sets EX to ((b<<16)/a)&0xffff. if a==0, sets b and EX to 0 instead. (treats b, a as unsigned)
    3, // OP_DVI = 0x07, // like DIV, but treat b, a as signed. Rounds towards 0
    3, // OP_MOD = 0x08, // sets b to b%a. if a==0, sets b to 0 instead.
    3, // OP_MDI = 0x09, // like MOD, but treat b, a as signed. (MDI -7, 16 == -7)
    1, // OP_AND = 0x0a, // sets b to b&a
    1, // OP_BOR = 0x0b, // sets b to b|a
    1, // OP_XOR = 0x0c, // sets b to b^a
    1, // OP_SHR = 0x0d, // sets b to b>>>a, sets EX to ((b<<16)>>a)&0xffff (logical shift)
    1, // OP_ASR = 0x0e, // sets b to b>>a, sets EX to ((b<<16)>>>a)&0xffff (arithmetic shift) (treats b as signed)
    1, // OP_SHL = 0x0f, // sets b to b<<a, sets EX to ((b<<a)>>16)&0xffff
    2, // OP_IFB = 0x10, // performs next instruction only if (b&a)!=0
    2, // OP_IFC = 0x11, // performs next instruction only if (b&a)==0
    2, // OP_IFE = 0x12, // performs next instruction only if b==a
    2, // OP_IFN = 0x13, // performs next instruction only if b!=a
    2, // OP_IFG = 0x14, // performs next instruction only if b>a
    2, // OP_IFA = 0x15, // performs next instruction only if b>a (signed)
    2, // OP_IFL = 0x16, // performs next instruction only if b<a
    2, // OP_IFU = 0x17, // performs next instruction only if b<a (signed)
    CostCalculator::INVALID_COST, // 0x18
    CostCalculator::INVALID_COST, // 0x19
    3, // OP_ADX = 0x1a, // sets b to b+a+EX, sets EX to 0x0001 if there is an overflow, 0x0 otherwise
    3, // OP_SBX = 0x1b, // sets b to b-a+EX, sets EX to 0xFFFF if there is an underflow, 0x0001 if there's an overflow, 0x0 otherwise
    CostCalculator::INVALID_COST, // 0x1c
    CostCalculator::INVALID_COST, // 0x1d
    2, // OP_STI = 0x1e, // sets b to a, then increases I and J by 1
    2, // OP_STD = 0x1f, // sets b to a, then decreases I and J by 1
};

const int CostCalculator::INVALID_COST{-1};

int CostCalculator::getNormalCost(Opcode o, uint8_t b, uint8_t a) {
    assert(a <= 0b111111);
    assert(b <= 0b11111);
    assert(o <= 0b11111);
    
    int result = s_normalCosts[o];
    if(result == INVALID_COST) {
        return INVALID_COST;
    }
    
    return result + argumentCost(a) + argumentCost(b);
}

int CostCalculator::getSpecialCost(Opcode o, uint8_t a) {
    assert(a <= 0b111111);
    assert(o <= 0b11111);
    
    unsigned result = 0;
    switch (o) {
        case OP_JSR: result = 3; break;    
        case OP_INT: result = 4; break;   
        case OP_IAG: result = 1; break;
        case OP_IAS: result = 1; break;
        case OP_RFI: result = 3; break;
        case OP_IAQ: result = 2; break;
        case OP_HWN: result = 2; break;
        case OP_HWQ: result = 4; break;
        case OP_HWI: result = 4; break;
            
        default:
            return INVALID_COST;
            break;
    }
    
    return result + argumentCost(a);
}

constexpr static int s_arguments[] = {
    0, 0, 0, 0, 0, 0, 0, 0, // 0x00-0x07 - register (A, B, C, X, Y, Z, I or J, in that order)
    0, 0, 0, 0, 0, 0, 0, 0, // 0x08-0x0f - [register]
    1, 1, 1, 1, 1, 1, 1, 1, // 0x10-0x17 - [register + next word]
    0, // 0x18 - (PUSH / [--SP]) if in b, or (POP / [SP++]) if in a
    0, // 0x19 - [SP] / PEEK
    1, // 0x1a - [SP + next word]
    0, 0, 0, // 0x1b-0x1d - SP, PC, EX
    1, // 0x1e - [next word]
    1, // 0x1f - next word (literal)
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // 0x20-0x3f - literal value 0xffff-0x1e (-1..30) (literal) (only for a)
};

int CostCalculator::argumentCost(uint8_t arg) {
    assert(arg <= 0x3f);
    return s_arguments[arg];
}