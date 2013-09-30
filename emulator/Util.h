//
//  Util.h
//  dcpu
//
//  Created by Artur Mazurek on 24.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_Util_h
#define dcpu_Util_h

static inline bool isLittleEndian() {
    const uint16_t check = 0xff00;
    const char* pCheck = reinterpret_cast<const char*>(&check);
    return *pCheck == 0x00;
}

#endif
