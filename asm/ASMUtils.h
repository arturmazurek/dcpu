//
//  ASMUtils.h
//  dcpu
//
//  Created by Artur Mazurek on 16.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_ASMUtils_h
#define dcpu_ASMUtils_h

#include <memory>

namespace std {
    template <typename T, typename... Args>
    static std::unique_ptr<T> make_unique(Args&&... args){
        return std::unique_ptr<T>{new T(std::forward<Args>(args)...)};
    }
}

#endif
