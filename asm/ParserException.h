//
//  ParserException.h
//  dcpu
//
//  Created by Artur Mazurek on 12/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_ParserException_h
#define dcpu_ParserException_h

#include <stdexcept>
#include <string>

class ParserException : public std::runtime_error {
public:
    ParserException(const std::string& what) : std::runtime_error{what} {}
};

#endif
