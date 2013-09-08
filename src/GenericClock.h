//
//  GenericClock.h
//  dcpu
//
//  Created by Artur Mazurek on 08.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __dcpu__GenericClock__
#define __dcpu__GenericClock__

#include "Hardware.h"
#include "Manufacturers.h"

class GenericClock : public Hardware {
public:
    GenericClock();
    
private:
    void doReceiveInterrupt(Core& from) override;
    
public:
    static constexpr uint32_t HARDWARE_ID = 0x12d0b402;
    static constexpr uint16_t VERSION = 1;
};

#endif /* defined(__dcpu__GenericClock__) */
