//
//  CoreException.h
//  dcpu
//
//  Created by Artur Mazurek on 27/08/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_DCPUException_h
#define dcpu_DCPUException_h

#include <stdexcept>
#include <string>

class DCPUException : public std::runtime_error {
public:
    DCPUException(const std::string& why) : std::runtime_error{why} {}
};

class TooManyInterruptsException : public DCPUException {
public:
    TooManyInterruptsException() : DCPUException{"Too many interrupts queued"} {}
};

class UnattachedHardware : public DCPUException {
public:
    UnattachedHardware(const std::string& reason) : DCPUException{reason} {}
};

#endif
