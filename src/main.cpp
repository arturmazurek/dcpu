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
    Instruction i{OP_ADD, 24, 12};
    cout << bitset<8>{i.raw[0]} << bitset<8>{i.raw[1]} << endl;
    
    return 0;
}

