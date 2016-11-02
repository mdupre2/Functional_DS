//
//  RBTree.hpp
//  FunctionalDS
//
//  Created by Mermer Dupree on 8/16/16.
//  Copyright © 2016 Mermer Dupree. All rights reserved.
//

#ifndef RBTree_hpp
#define RBTree_hpp

#include <stdio.h>
#include<iostream>
#include"List.hpp"
#include<string>
#include "RBNode.hpp"
#include "enums.hpp"
#include "Path.hpp"
using namespace std;



extern string to_String(Color c);
extern string to_String(Comparison c);



/********** RBTree ************/
template <class Key, class Value> class RBTree{
private:
    RBNode<Key, Value>* root;
    int count;
    
public:
    RBTree();
    RBTree(Key key, Value val);
    RBTree(RBNode<Key, Value>* root, int count);
    RBTree<Key, Value>* insert(Key key, Value val);
    Value get(Key key);
    void print();
};






#endif /* RBTree_hpp */
