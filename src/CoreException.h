//
//  CoreException.h
//  dcpu
//
//  Created by Artur Mazurek on 27/08/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_CoreException_h
#define dcpu_CoreException_h

#include <stdexcept>
#include <string>

class CoreException : public std::runtime_error {
public:
    CoreException(const std::string& why) : std::runtime_error{why} {}
};

class TooManyInterruptsException : public CoreException {
public:
    TooManyInterruptsException() : CoreException{"Too many interrupts queued"} {}
};

#endif
