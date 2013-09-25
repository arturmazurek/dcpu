//
//  test_main.cpp
//  dcpu
//
//  Created by Artur Mazurek on 12/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "AssemblerTests.h"
#include "BaseTests.h"
#include "CoreTests.h"
#include "LexerTests.h"
#include "OpcodesTests.h"
#include "ParserTests.h"
#include "SpecialOpcodesTests.h"
#include "TestsHolder.h"
#include "VisitorsTests.h"

using namespace std;

int main(int argc, const char * argv[])
{
    ADD_TESTS(BaseTests);
    ADD_TESTS(OpcodesTests);
    ADD_TESTS(SpecialOpcodesTests);
    ADD_TESTS(CoreTests);
    
    ADD_TESTS(LexerTests);
    ADD_TESTS(ParserTests);
    ADD_TESTS(VisitorsTests);
    ADD_TESTS(AssemblerTests);
    
    TestsHolder::instance().runTests();
    
    return 0;
}