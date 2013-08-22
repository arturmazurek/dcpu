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
    
    Instruction m[] = {
        {OP_SET, 0x01, 0x22},
        {OP_SET, 0x00, 0x20},
        {OP_SET, 0x02, 0x00}
    };
    c.setInstructions(m, sizeof(m) / sizeof(*m));
    
    for(int i = 0; i < (sizeof(m) / sizeof(*m)); ++i) {
        c.doCycle();
        cout << endl;
        c.printRegisters();
    }
    
    cout << endl;
    c.printMemory(0, 10);
    
    return 0;
}

