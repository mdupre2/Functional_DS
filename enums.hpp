//
//  enums.hpp
//  FunctionalDS
//
//  Created by Mermer Dupree on 8/18/16.
//  Copyright © 2016 Mermer Dupree. All rights reserved.
//

#ifndef enums_hpp
#define enums_hpp

#include <stdio.h>
#include <string>

enum Color{ Red, Black };

enum ChildType{ RightRight, RightLeft, LeftRight, LeftLeft };

enum Comparison{ Less, Greater, Equal };

//template <class Key, class Value> class RBNode;
/*
std::string to_String(Comparison c){
    switch (c) {
        case Less:
            return "Less";
            break;
        case Greater:
            return "Greater";
            break;
        case Equal:
            return "Equal";
            break;
        default:
            return "Error";
            break;
    }
}

std::string to_String(Color c){
    switch (c) {
        case Red:
            return "Red";
            break;
        case Black:
            return "Black";
            break;
        default:
            return "Error";
            break;
    }
}
*/
#endif /* enums_hpp */
