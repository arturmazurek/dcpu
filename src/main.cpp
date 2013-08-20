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

int main(int argc, const char * argv[])
{
    Core c;
    c.resetState();
    c.printRegisters();
    
    std::cout << std::endl;
    Instruction::Value v = {OP_MUL, 1, 63};
    std::cout << std::bitset<16>(v.raw) << std::endl;
    
    return 0;
}

