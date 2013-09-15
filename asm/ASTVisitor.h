//
//  ASTVisitor.h
//  dcpu
//
//  Created by Artur Mazurek on 14.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __dcpu__ASTVisitor__
#define __dcpu__ASTVisitor__

class ASTVisitor {
public:
    virtual ~ASTVisitor() {}
};

template <typename T>
class ASTVisitorType {
public:
    virtual void visit(T&) = 0;
};

#endif /* defined(__dcpu__ASTVisitor__) */
