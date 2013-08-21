//
//  main.cpp
//  dcpu
//
//  Created by Artur Mazurek on 20.08.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include <bitset>
#include <iostream>

#include "Core.h"
#include "Opcodes.h"

using namespace std;

int main(int argc, const char * argv[])
{
    Core c;
    c.resetState();
    
    c.printRegisters();
    cout << endl;
    
    Instruction m[] = {
        {OP_SET, 0x02, 0x07}
    };
    c.setInstructions(m, sizeof(m) / sizeof(*m));
    
    c.doCycle();
    
    cout << endl;
    c.printRegisters();
    
    return 0;
}

