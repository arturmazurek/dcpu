//
//  LexerException.h
//  dcpu
//
//  Created by Artur Mazurek on 11.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_LexerException_h
#define dcpu_LexerException_h

#include <stdexcept>
#include <string>

class LexerException : public std::runtime_error {
public:
    LexerException(const std::string& why) : std::runtime_error{why} {}
};

#endif
