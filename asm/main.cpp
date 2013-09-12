//
//  main.cpp
//  dcpu-asm
//
//  Created by Artur Mazurek on 11.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include <iostream>
#include <sstream>

#include "Lexer.h"
#include "LexerException.h"

using std::cout;
using std::endl;

int main(int argc, const char * argv[])
{
    std::stringstream ss;
    ss << " ddd ;commented out \n  dsa ddd 76 s76 repeat 076 repeat "; // should throw on the last but one

    Lexer l{ss};

    try {
        char tok = 0;
        while((tok = l.nextToken()) != Lexer::TOK_EOF) {
            switch (tok) {
                case Lexer::TOK_IDENTIFIER:
                    cout << "Identifier: " << l.identifier() << endl;
                    break;
                    
                case Lexer::TOK_NUMBER:
                    cout << "Number: " << l.number() << endl;
                    break;
                    
                case Lexer::TOK_REPEAT:
                    cout << "Repeat" << endl;
                    break;
                    
                default:
                    cout << "Unknown something: " << tok << endl;
                    break;
            }
        }
    } catch (LexerException& e) {
        cout << "Lexer exception: " << e.what() << endl;
    }
    
    return 0;
}

