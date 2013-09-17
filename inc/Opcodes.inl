//
//  Opcodes.inl
//  dcpu
//
//  Created by Artur Mazurek on 17/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef OPCODES_INL
#error "don't include this file, if you're not sure you want it"
#endif

OPCODE(NONE, 0x00), // special

// Normal
OPCODE(SET, 0x01), // sets b to a
OPCODE(ADD, 0x02), // sets b to b+a, sets EX to 0x0001 if there's an overflow, 0x0 otherwise
OPCODE(SUB, 0x03), // sets b to b-a, sets EX to 0xffff if there's an underflow, 0x0 otherwise
OPCODE(MUL, 0x04), // sets b to b*a, sets EX to ((b*a)>>16)&0xffff (treats b, a as unsigned)
OPCODE(MLI, 0x05), // like MUL, but treat b, a as signed
OPCODE(DIV, 0x06), // sets b to b/a, sets EX to ((b<<16)/a)&0xffff. if a==0, sets b and EX to 0 instead. (treats b, a as unsigned)
OPCODE(DVI, 0x07), // like DIV, but treat b, a as signed. Rounds towards 0
OPCODE(MOD, 0x08), // sets b to b%a. if a==0, sets b to 0 instead.
OPCODE(MDI, 0x09), // like MOD, but treat b, a as signed. (MDI -7, 16 == -7)
OPCODE(AND, 0x0a), // sets b to b&a
OPCODE(BOR, 0x0b), // sets b to b|a
OPCODE(XOR, 0x0c), // sets b to b^a
OPCODE(SHR, 0x0d), // sets b to b>>>a, sets EX to ((b<<16)>>a)&0xffff (logical shift)
OPCODE(ASR, 0x0e), // sets b to b>>a, sets EX to ((b<<16)>>>a)&0xffff (arithmetic shift) (treats b as signed)
OPCODE(SHL, 0x0f), // sets b to b<<a, sets EX to ((b<<a)>>16)&0xffff
OPCODE(IFB, 0x10), // performs next instruction only if (b&a)!=0
OPCODE(IFC, 0x11), // performs next instruction only if (b&a)==0
OPCODE(IFE, 0x12), // performs next instruction only if b==a
OPCODE(IFN, 0x13), // performs next instruction only if b!=a
OPCODE(IFG, 0x14), // performs next instruction only if b>a
OPCODE(IFA, 0x15), // performs next instruction only if b>a (signed)
OPCODE(IFL, 0x16), // performs next instruction only if b<a
OPCODE(IFU, 0x17), // performs next instruction only if b<a (signed)
OPCODE(ADX, 0x1a), // sets b to b+a+EX, sets EX to 0x0001 if there is an overflow, 0x0 otherwise
OPCODE(SBX, 0x1b), // sets b to b-a+EX, sets EX to 0xFFFF if there is an underflow, 0x0001 if there's an overflow, 0x0 otherwise
OPCODE(STI, 0x1e), // sets b to a, then increases I and J by 1
OPCODE(STD, 0x1f), // sets b to a, then decreases I and J by 1

// Special
OPCODE(JSR, 0x01), // pushes the address of the next instruction to the stack, then sets PC to a
OPCODE(INT, 0x08), // triggers a software interrupt with message a
OPCODE(IAG, 0x09), // sets a to IA
OPCODE(IAS, 0x0a), // sets IA to a
OPCODE(RFI, 0x0b), // disables interrupt queueing, pops A from the stack, then pops PC from the stack
OPCODE(IAQ, 0x0c), // if a is nonzero, interrupts will be added to the queue instead of triggered. if a is zero, interrupts will be triggered as normal again
OPCODE(HWN, 0x10), // sets a to number of connected hardware devices
OPCODE(HWQ, 0x11), // sets A, B, C, X, Y registers to information about hardware [...]
OPCODE(HWI, 0x12)  // sends an interrupt to hardware a

