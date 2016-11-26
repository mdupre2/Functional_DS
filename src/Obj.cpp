//
//  Obj.cpp
//  FunctionalDS
//
//  Created by Mermer Dupree on 11/17/16.
//  Copyright © 2016 Mermer Dupree. All rights reserved.
//

#include "Obj.hpp"

unsigned long long Obj::mem = 0;

Obj::Obj(){
    mem += sizeof(Obj);
}
Obj::~Obj(){
    mem -= sizeof(Obj);
}


unsigned long long Obj::memoryUsed(){
    return mem;
}
