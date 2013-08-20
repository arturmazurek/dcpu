//
//  main.cpp
//  dcpu
//
//  Created by Artur Mazurek on 20.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include <iostream>

#include "Core.h"

int main(int argc, const char * argv[])
{
    Core c;
    c.resetState();
    c.printRegisters();
    
    return 0;
}

