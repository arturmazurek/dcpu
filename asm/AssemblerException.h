//
//  AssemblerException.h
//  dcpu
//
//  Created by Artur Mazurek on 16.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_AssemblerException_h
#define dcpu_AssemblerException_h

#include <stdexcept>
#include <string>

class AssemblerException : public std::runtime_error {
public:
    AssemblerException(const std::string& what) : std::runtime_error{what} {}
};

#endif
