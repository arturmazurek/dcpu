//
//  GenericClock.cpp
//  dcpu
//
//  Created by Artur Mazurek on 08.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "GenericClock.h"

const uint16_t GenericClock::VERSION{1};
const uint32_t GenericClock::HARDWARE_ID{0x12d0b402};

GenericClock::GenericClock() : Hardware{HARDWARE_ID, VERSION, Manufacturers::NYA_ELEKTRISKA} {
    
}

void GenericClock::doReceiveInterrupt(Core& from) {
    
}