//
//  emulator_main.c
//  dcpu
//
//  Created by Artur on 30/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include <cstdint>
#include <cstdlib>

#include <fstream>
#include <iostream>
#include <vector>

#include "Core.h"

using namespace std;

enum ReturnCodes {
    RETURN_SUCCESS = 0,
    RETURN_WRONG_INVOCATION = 1,
    RETURN_FILE_ERROR = 2,
    RETURN_CORE_EXCEPTION = 3,
};

static constexpr int BUF_SIZE = 1024;

void mainLoop(Core& core);
void printHelp();

int main(int argc, char** argv) {
    if(argc < 2) {
        printHelp();
        return RETURN_WRONG_INVOCATION;
    }
    
    ifstream inputFile{argv[1], ios_base::in | ios_base::binary};
    if(inputFile.bad()) {
        cout << "Could not open " << argv[1] << endl;
        return RETURN_FILE_ERROR;
    }

    vector<uint16_t> memory;
    uint16_t buffer[BUF_SIZE];
    
    while(true) {
        memset(buffer, 0, BUF_SIZE * sizeof(uint16_t));
        auto size = inputFile.readsome(reinterpret_cast<char*>(buffer), BUF_SIZE * sizeof(uint16_t));
        if(size <= 0) {
            break;
        }
        memory.insert(memory.end(), buffer, buffer + (size / sizeof(uint16_t)));
    }
    
    Core core;
    core.setMemory(memory);
    mainLoop(core);
    
    return RETURN_SUCCESS;
}

void printHelp() {
    cout << "dcpu-emu - an emulator of DCPU-16\n";
    cout << "    dcpu-emu <binary_path>" << endl;
}

void mainLoop(Core& core) {
    string in;
    while(true) {
        cin >> in;
        
        if(in == "q") {
            cout << "Goodbye" << endl;
            break;
        }
    }
}
