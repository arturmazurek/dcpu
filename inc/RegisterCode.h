//
//  RegisterCode.h
//  dcpu
//
//  Created by Artur Mazurek on 17.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_RegisterCode_h
#define dcpu_RegisterCode_h

enum RegisterCode {
    REG_A = 0x00,
    REG_B = 0x01,
    REG_C = 0x02,
    REG_X = 0x03,
    REG_Y = 0x04,
    REG_Z = 0x05,
    REG_I = 0x06,
    REG_J = 0x07,
    REG_SP = 0x1b,
    REG_PC = 0x1c,
    REG_EX = 0x1d
};

#endif
