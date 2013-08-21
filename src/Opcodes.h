//
//  Opcodes.h
//  dcpu
//
//  Created by Artur Mazurek on 20.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_opcodes_h
#define dcpu_opcodes_h

enum Opcode {
    OP_NONE = 0x00, // special
    
    // Normal 
    OP_SET = 0x01, // sets b to a
    OP_ADD = 0x02, // sets b to b+a, sets EX to 0x0001 if there's an overflow, 0x0 otherwise
    OP_SUB = 0x03, // sets b to b-a, sets EX to 0xffff if there's an underflow, 0x0 otherwise
    OP_MUL = 0x04, // sets b to b*a, sets EX to ((b*a)>>16)&0xffff (treats b, a as unsigned)
    OP_MLI = 0x05, // like MUL, but treat b, a as signed
    OP_DIV = 0x06, // sets b to b/a, sets EX to ((b<<16)/a)&0xffff. if a==0, sets b and EX to 0 instead. (treats b, a as unsigned)
    OP_DVI = 0x07, // like DIV, but treat b, a as signed. Rounds towards 0
    OP_MOD = 0x08, // sets b to b%a. if a==0, sets b to 0 instead.
    OP_MDI = 0x09, // like MOD, but treat b, a as signed. (MDI -7, 16 == -7)
    OP_AND = 0x0a, // sets b to b&a
    OP_BOR = 0x0b, // sets b to b|a
    OP_XOR = 0x0c, // sets b to b^a
    OP_SHR = 0x0d, // sets b to b>>>a, sets EX to ((b<<16)>>a)&0xffff (logical shift)
    OP_ASR = 0x0e, // sets b to b>>a, sets EX to ((b<<16)>>>a)&0xffff (arithmetic shift) (treats b as signed)
    OP_SHL = 0x0f, // sets b to b<<a, sets EX to ((b<<a)>>16)&0xffff
    OP_IFB = 0x10, // performs next instruction only if (b&a)!=0
    OP_IFC = 0x11, // performs next instruction only if (b&a)==0
    OP_IFE = 0x12, // performs next instruction only if b==a
    OP_IFN = 0x13, // performs next instruction only if b!=a
    OP_IFG = 0x14, // performs next instruction only if b>a
    OP_IFA = 0x15, // performs next instruction only if b>a (signed)
    OP_IFL = 0x16, // performs next instruction only if b<a
    OP_IFU = 0x17, // performs next instruction only if b<a (signed)
    OP_ADX = 0x1a, // sets b to b+a+EX, sets EX to 0x0001 if there is an overflow, 0x0 otherwise
    OP_SBX = 0x1b, // sets b to b-a+EX, sets EX to 0xFFFF if there is an underflow, 0x0001 if there's an overflow, 0x0 otherwise
    OP_STI = 0x1e, // sets b to a, then increases I and J by 1
    OP_STD = 0x1f, // sets b to a, then decreases I and J by 1
    
    // Special
    OP_JSR = 0x01, // pushes the address of the next instruction to the stack, then sets PC to a
    OP_INT = 0x08, // triggers a software interrupt with message a
    OP_IAG = 0x09, // sets a to IA
    OP_IAS = 0x0a, // sets IA to a
    OP_RFI = 0x0b, // disables interrupt queueing, pops A from the stack, then pops PC from the stack
    OP_IAQ = 0x0c, // if a is nonzero, interrupts will be added to the queue instead of triggered. if a is zero, interrupts will be triggered as normal again
    OP_HWN = 0x10, // sets a to number of connected hardware devices
    OP_HWQ = 0x11, // sets A, B, C, X, Y registers to information about hardware [...]
    OP_HWI = 0x12  // sends an interrupt to hardware a
};

#endif
