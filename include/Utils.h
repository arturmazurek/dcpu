//
//  Utils.h
//  dcpu
//
//  Created by Artur on 30/09/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef dcpu_Utils_h
#define dcpu_Utils_h

#include <memory>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(*x))

namespace std {
    template <typename T, typename... Args>
    static std::unique_ptr<T> make_unique(Args&&... args){
        return std::unique_ptr<T>{new T(std::forward<Args>(args)...)};
    }
}

#endif
