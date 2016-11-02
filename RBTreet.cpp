//
//  RBTree.cpp
//  FunctionalDS
//
//  Created by Mermer Dupree on 8/16/16.
//  Copyright © 2016 Mermer Dupree. All rights reserved.
//

#include "RBTree.hpp"


/*   Public Funtions   */

template <class Key, class Value>
RBTree<Key, Value>::RBTree(){
    root = NULL;
    count = 0;
}

template <class Key, class Value>
RBTree<Key, Value>::RBTree(Key key, Value val){
    root = new RBNode<Key, Value>(Black, key, val, NULL, NULL, 1),
    count = 1;
}

template <class Key, class Value>
RBTree<Key, Value>::RBTree(RBNode<Key, Value>* root, int count){
    this->root = root;
    this->count = count;
}

template <class Key, class Value>
void RBTree<Key, Value>::print(){
    if (root != NULL){
        root->print_recursive();
    }
    else{
        cout << "empty" << endl;
    }
}

template <class Key, class Value>
RBTree<Key, Value>* RBTree<Key, Value>::insert(Key key, Value val){
    if (root == NULL){
        return new RBTree<Key,Value>(key, val);
    }
    Path<Key,Value>* path = root->getPath(key, val);
    return new RBTree<Key,Value> (path->top(), 1);
}


/*   Private Funtions   */
