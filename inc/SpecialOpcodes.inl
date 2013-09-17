//
//  SpecialOpcodes.h
//  dcpu
//
//  Created by Artur Mazurek on 17.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef OPCODES_INL
#error "don't include this file, if you're not sure you want it"
#endif

// Special
OPCODE(JSR, 0x01), // pushes the address of the next instruction to the stack, then sets PC to a
OPCODE(INT, 0x08), // triggers a software interrupt with message a
OPCODE(IAG, 0x09), // sets a to IA
OPCODE(IAS, 0x0a), // sets IA to a
OPCODE(RFI, 0x0b), // disables interrupt queueing, pops A from the stack, then pops PC from the stack
OPCODE(IAQ, 0x0c), // if a is nonzero, interrupts will be added to the queue instead of triggered. if a is zero, interrupts will be triggered as normal again
OPCODE(HWN, 0x10), // sets a to number of connected hardware devices
OPCODE(HWQ, 0x11), // sets A, B, C, X, Y registers to information about hardware [...]
OPCODE(HWI, 0x12), // sends an interrupt to hardware a