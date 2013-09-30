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
#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "Core.h"

using namespace std;

enum ReturnCodes {
    RETURN_SUCCESS = 0,
    RETURN_PROGRAM_ERROR = 1,
    RETURN_WRONG_INVOCATION = 2,
    RETURN_FILE_ERROR = 3,
    RETURN_CORE_EXCEPTION = 4,
};

static constexpr int BUF_SIZE = 1024;

void mainLoop(Core& core);
void printHelp();
void printMainLoopHelp();

int main(int argc, char** argv) {
    if(argc < 2) {
        printHelp();
        return RETURN_WRONG_INVOCATION;
    }
    
    ifstream inputFile{argv[1], ios_base::in | ios_base::binary | ios::ate};
    if(inputFile.bad()) {
        cout << "Could not open " << argv[1] << endl;
        return RETURN_FILE_ERROR;
    }

    auto size = inputFile.tellg();
    std::vector<uint16_t> memory(size / sizeof(uint16_t));
    inputFile.seekg(0, ios::beg);
    inputFile.read(reinterpret_cast<char*>(memory.data()), size);
    inputFile.close();
    
    Core core;
    core.resetState();
    core.setMemory(memory);
    
    try {
        mainLoop(core);
    } catch(const std::runtime_error& e) {
        cout << "Execution error - " << e.what() << endl;
        return RETURN_PROGRAM_ERROR;
    }
    
    return RETURN_SUCCESS;
}

void printHelp() {
    cout << "dcpu-emu - an emulator of DCPU-16\n";
    cout << "    dcpu-emu <binary_path>" << endl;
}

void printMainLoopHelp() {
    cout << "Help unavailable yet." << endl;
}

void mainLoop(Core& core) {
    cout << "DCPU-16 running:" << endl;
    
    string in;
    while(true) {
        getline(cin, in);
        
        if(in == "q" || in == "quit") {
            cout << "Goodbye" << endl;
            break;
        } else if(in == "c" || in == "cycle") {
            core.cycle();
        } else if(in == "pr" || in == "print registers") {
            core.printRegisters();
        } else if(in == "h" || in == "help") {
            printMainLoopHelp();
        } else if(in.find("pm") == 0 || in.find("print memory") == 0) {
            cout << "Enter index and length" << endl;
            
            string s;
            getline(cin, s);
            stringstream ss{s};
            
            uint16_t begin = 0, len = 0;
            if(!(ss >> begin) || !(ss >> len)) {
                cout << "Wrong line encountered - " << s << endl;
                continue;
            }
            core.printMemory(begin, begin + len);
        } else {
            if(in.size()) {
                cout << "Use 'h' or 'help' for help on commands, 'q' or 'quit' to quit" << endl;
            }
        }
    }
}
