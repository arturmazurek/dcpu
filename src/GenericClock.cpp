//
//  GenericClock.cpp
//  dcpu
//
//  Created by Artur Mazurek on 08.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "GenericClock.h"

GenericClock::GenericClock() : Hardware{HARDWARE_ID, VERSION, Manufacturers::NYA_ELEKTRISKA} {
    
}

void GenericClock::doReceiveInterrupt(Core& from) {
    
}