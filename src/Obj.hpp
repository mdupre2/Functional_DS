//
//  Obj.hpp
//  FunctionalDS
//
//  Created by Mermer Dupree on 11/17/16.
//  Copyright © 2016 Mermer Dupree. All rights reserved.
//

#ifndef Obj_hpp
#define Obj_hpp

#include <stdio.h>

class Obj{
    char memblock[2048];
    public:
    Obj();
    ~Obj();
    static unsigned long long memoryUsed();
    static unsigned long long mem;
};

#endif /* Obj_hpp */
