//
//  main_assembler.c
//  dcpu
//
//  Created by Artur on 30/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "Assembler.h"
#include "Lexer.h"
#include "Utils.h"

using namespace std;

enum ReturnCodes {
    RETURN_SUCCESS = 0,
    RETURN_WRONG_INVOCATION = 1,
    RETURN_FILE_ERROR = 2,
    RETURN_ASSEMBLY_ERROR = 3,
};

void printHelp();

int main(int argc, char** argv) {
    if(argc < 2) {
        printHelp();
        return RETURN_WRONG_INVOCATION;
    }
    
    string input = argv[1];
    string output = argc > 2 ? argv[2] : argv[1];
    if(output == input) {
        auto pos = output.find(".");
        if(pos != string::npos) {
            output = output.substr(0, pos);
        }
    }
    
    ifstream inputFile{input};
    if(inputFile.bad()) {
        cout << "Could not open input file: " << input << endl;
        return RETURN_FILE_ERROR;
    }
    
    ofstream outputFile{output, ios_base::out | ios_base::binary};
    if(outputFile.bad()) {
        cout << "Could not open output file: " << output << endl;
        return RETURN_FILE_ERROR;
    }
    
    Assembler a;
    a.setLexer(make_unique<Lexer>(inputFile));
    try {
        a.assemble();
    } catch (const runtime_error& e) {
        cout << "Assembly failed - " << e.what() << std::endl;
        return RETURN_ASSEMBLY_ERROR;
    }
    
    outputFile.write(reinterpret_cast<const char*>(a.binary().data()), a.binary().size() * sizeof(uint16_t));
    
    return RETURN_SUCCESS;
}

void printHelp() {
    cout << "dcpu-asm - a simple assembler for DCPU-16\n";
    cout << "Usage:\n";
    cout << "    dcpu-asm <input> <output(optional)>" << endl;
}