//
//  RBNode.hpp
//  FunctionalDS
//
//  Created by Mermer Dupree on 8/18/16.
//  Copyright © 2016 Mermer Dupree. All rights reserved.
//

#ifndef RBNode_hpp
#define RBNode_hpp

#include <stdio.h>
#include <iostream>
#include "List.hpp"
#include "enums.hpp"
#include "Path.hpp"
#include <sstream>
using namespace std;

//unsigned long long rbmem = 0;

/********** RBNode ************/
template <class Key, class Value> class RBNode{
public:
    Color color;
    Key key;
    Value val;
    RBNode* left;
    RBNode* right;
    int count;
    bool isNull = false;
    static unsigned long long mem;
    
    RBNode(Color color, Key key, Value val, RBNode* left, RBNode* right, int count);
    RBNode();
    RBNode<Key, Value>* recolor(Color color);
    RBNode<Key, Value>* modify(Color color, RBNode* left, RBNode* right);
    RBNode<Key, Value>* modify(RBNode* left, RBNode* right);
    RBNode<Key, Value>* modify_key(Key key);
    RBNode<Key, Value>* modify_val(Value val);
    RBNode<Key, Value>* copy();
    
    void print();
    string dotfile_recurs(string nodeName);
    void print_recursive();
    int blackHeight();
    bool hasRedParentChild();
    bool isLeaf();
    RBNode<Key, Value>* search(Key key);
    Path<Key,Value>* db_rebalance(Path<Key,Value>*);
    Path<Key,Value>* getPath(Key key, Value val);
    Path<Key,Value>* getPath(RBNode<Key,Value>* node);
    Path<Key,Value>* getPath_delete(Key key);
    Path<Key,Value>* getPath_delete_least();
    Path<Key,Value>* getPath_doubleBlack(RBNode<Key,Value>* node);
    Key least();
    Key greatest();
    Value least_val();
    Value greatest_val();
    unsigned long long memoryUsed();
};

template <class Key, class Value>
unsigned long long RBNode<Key, Value>::mem = 0;

template <class Key, class Value>
RBNode<Key, Value>::RBNode(Color color, Key key, Value val, RBNode* left, RBNode* right, int count){
    this->color = color;
    this->key = key;
    this->val = val;
    this->left = left;
    this->right = right;
    this->count = count;
    this->isNull = false;
    mem += sizeof(RBNode<Key,Value>);
}

template <class Key, class Value>
RBNode<Key, Value>::RBNode(){
    this->left = NULL;
    this->right = NULL;
    this->isNull = true;
    color = Black;
    mem += sizeof(RBNode<Key,Value>);
}

template <class Key, class Value>
unsigned long long RBNode<Key, Value>::memoryUsed(){
    return mem;
}

template <class Key, class Value>
RBNode<Key, Value>* RBNode<Key, Value>::recolor( Color c){
    return new RBNode(c, key, val, left, right, count);
}

template <class Key, class Value>
RBNode<Key, Value>* RBNode<Key, Value>::copy(){
    return new RBNode(color, key, val, left, right, count);
}

template <class Key, class Value>
RBNode<Key, Value>* RBNode<Key, Value>::modify( Color c, RBNode* l, RBNode* r){
    return new RBNode(c, key, val, l, r, count);
}

template <class Key, class Value>
RBNode<Key, Value>* RBNode<Key, Value>::modify(RBNode* l, RBNode* r){
    return new RBNode(color, key, val, l, r, count);
}

template <class Key, class Value>
RBNode<Key, Value>* RBNode<Key, Value>::modify_key(Key k){
    return new RBNode(color, k, val, left, right, count);
}

template <class Key, class Value>
RBNode<Key, Value>* RBNode<Key, Value>::modify_val(Value v){
    return new RBNode(color, key, v, left, right, count);
}

template <class Key, class Value>
void RBNode<Key, Value>::print(){
    cout << "Color: " << ((color == Red) ? "Red" : "Black" ) << endl;
    cout << "Key: " << key << endl;
    cout << "Val: " << val << endl;
    cout << "\n";
}

template <class Key, class Value>
void RBNode<Key, Value>::print_recursive(){
    if (left != NULL){
        left->print_recursive();
    }
    this->print();
    if(right != NULL){
        right->print_recursive();
    }
}

template <class Key, class Value>
string RBNode<Key, Value>::dotfile_recurs(string nodeName){
    string s;
    string l_nn = nodeName + "0";
    string r_nn = nodeName + "1";
    s += nodeName;
    s += "->";
    s += l_nn;
    s += "\n";
    s += nodeName;
    s += "->";
    s += r_nn;
    s += "\n";
    s+= nodeName;
    s += "[label=\"";
    stringstream ss;
    ss << key;
    s += ss.str();
    s += "\",style=filled, fillcolor=";
    if (color == Red){
        s += "red, ";
    }else{
        s += "black, fontcolor=white ";
    }
    s += "]\n";
    
    
    if (left != NULL && !left->isNull){
        s += left->dotfile_recurs(l_nn);
    }
    else if (left != NULL && left->isNull){
        s += l_nn;
        s += "[label=\"Double-Black NULL\", shape=rectangle, style=filled, fillcolor=black, fontcolor=white]\n";
    }
    else{
        s += l_nn;
        s += "[label=\"NULL\", shape=rectangle, style=filled, fillcolor=black, fontcolor=white]\n";
    }
    
    if (right != NULL && !right->isNull){
        s += right->dotfile_recurs(r_nn);
    }
    else if (right != NULL && right->isNull){
        s += r_nn;
        s += "[label=\"Double-Black NULL\", style=filled, shape=rectangle, fillcolor=black, fontcolor=white]\n";
    }
    else{
        s += r_nn;
        s += "[label=\"NULL\", shape=rectangle, style=filled, fillcolor=black, fontcolor=white]\n";
    }
    
    return s;
}

template <class Key, class Value>
int RBNode<Key, Value>::blackHeight(){
    int lbh;
    int rbh;
    if (left != NULL){
        lbh = left->blackHeight();
    }else{
        lbh = 1;
    }
    
    if(right != NULL){
        rbh = right->blackHeight();
    }else{
        rbh = 1;
    }
    
    if (rbh == -1 || lbh == -1){
        return -1;
    }
    else if (rbh != lbh){
        return -1;
    }
    else if(color == Black){
        return rbh + 1;
    }else{
        return rbh;
    }
}

template <class Key, class Value>
bool RBNode<Key, Value>::hasRedParentChild(){
    bool l,r;
    if (color == Red){
        if (left != NULL && left->color == Red){
            return true;
        }
        if (right != NULL && right->color == Red){
            return true;
        }
    }
    if (left != NULL){
        l = left->hasRedParentChild();
    }else{
        l = false;
    }
    if (right != NULL){
        r = right->hasRedParentChild();
    }else{
        r = false;
    }
    return (l || r);
    
}

template <class Key, class Value>
bool RBNode<Key, Value>::isLeaf(){
    return (left == NULL && right == NULL);
}

template <class Key, class Value>
RBNode<Key, Value>* RBNode<Key, Value>::search(Key k){
    RBNode<Key, Value>* l;
    RBNode<Key, Value>* r;
    
    if (key == k){
        return this;
    }
    if (left == NULL){
        l = NULL;
    }else{
        l = left->search(k);
    }
    if (right == NULL){
        r = NULL;
    }else{
        r = right->search(k);
    }
    if (r == NULL && l == NULL){
        return NULL;
    }
    if (r == NULL){
        return l;
    }
    if (l == NULL){
        return r;
    }
    return r;
    
}

template <class Key, class Value>
Path<Key, Value>* RBNode<Key, Value>::getPath(Key newKey, Value newVal){
    
    if (newKey < key){ // less
        if (left == NULL){
            auto newNode = new RBNode<Key, Value>(Red, newKey, newVal, NULL, NULL, 1);
            auto ret =  new Path<Key, Value> (newNode);
            if (color == Red){
                ret->needsRebalancing = true;
            }
            return ret->push(this->modify(newNode, right), Less);
        }else{
            auto partialPath = left->getPath(newKey,newVal);
            auto ret = partialPath->push(this->modify(partialPath->top(), right), Less);
            return ret;
        }
    }
    
    else if(newKey == key){ // equal
        auto newNode = this->modify_val(newVal);
        auto ret =  new Path<Key, Value> (newNode);
        return ret;
    }
    
    else{ // greater
        if (right == NULL){
            auto newNode = new RBNode(Red, newKey, newVal, NULL, NULL, 1);
            auto ret = new Path<Key, Value> (newNode);
            if (color == Red){
                ret->needsRebalancing = true;
            }
            return ret->push(this->modify(left, newNode), Greater);
            
        }else{
            auto partialPath = right->getPath(newKey,newVal);
            auto ret = partialPath->push(this->modify(left, partialPath->top()), Greater);
            return ret;
        }
    }
}

template <class Key, class Value>
Path<Key, Value>* RBNode<Key, Value>::getPath(RBNode<Key, Value>* node){
    
    if (node->key < key){ // less
        if (left == NULL){
            
            auto ret =  new Path<Key, Value> (node);
            if (color == Red && node->color == Red){
                ret->needsRebalancing = true;
            }
            return ret->push(this->modify(node, right), Less);
        }else{
            auto partialPath = left->getPath(node);
            auto ret = partialPath->push(this->modify(partialPath->top(), right), Less);
            return ret;
        }
    }
    
    else if(node->key == key){ // equal
        return  new Path<Key, Value> (this->modify_val(node->val));
    }
    
    else{ // greater
        if (right == NULL){
            auto ret = new Path<Key, Value> (node);
            if (color == Red && node->color == Red){
                ret->needsRebalancing = true;
            }
            return ret->push(this->modify(left, node), Greater);
        }else{
            auto partialPath = right->getPath(node);
            auto ret = partialPath->push(this->modify(left, partialPath->top()), Greater);
            return ret;
        }
    }
}

template <class Key, class Value>
Path<Key, Value>* RBNode<Key, Value>::getPath_delete(Key dKey){
    
    if (dKey < key){ // less
        if (left == NULL){
            return NULL;
        }else{
            auto partialPath = left->getPath_delete(dKey);
            if (partialPath == NULL){
                return NULL;
            }
            auto ret = partialPath->push(this->modify(partialPath->top(), right), Less);
            return ret;
        }
    }
    else if(dKey == key) {// equal
        
        if (left != NULL && right != NULL){
            auto partialPath = right->getPath_delete(right->least());
            auto deleted = partialPath->deleted;
            auto replacement = new RBNode<Key, Value>(color, deleted->key, deleted->val, left, partialPath->top(),  count);
            return partialPath->push(replacement, Greater);
        }
        else if (left == NULL && right != NULL){
            if (color == Red && right->color == Red){
                auto replacement = right->modify(Red, NULL, NULL);
                auto ret = new Path<Key, Value>(replacement);
                ret->deleted = this;
                ret->needsRebalancing = false;
                return ret;
            }else if (color == Red || right->color == Red){
                auto replacement = right->modify(Black, NULL, NULL);
                auto ret = new Path<Key, Value>(replacement);//
                ret->deleted = this;
                ret->needsRebalancing = false;
                return ret;
            }else{ //Both Black
                auto replacement = right->modify(Black, NULL, NULL);
                auto ret = new Path<Key, Value>(replacement);//
                ret->deleted = this;
                ret->needsRebalancing = true;
                return ret;
            }
        }else if (left != NULL && right == NULL){
            if (color == Red && left->color == Red){
                auto replacement = left->modify(Red, NULL, NULL);
                auto ret = new Path<Key, Value>(replacement);//
                ret->deleted = this;
                ret->needsRebalancing = false;
                return ret;
            }else if (color == Red || left->color == Red){
                auto replacement = left->modify(Black, NULL, NULL);
                auto ret = new Path<Key, Value>(replacement);//
                ret->deleted = this;
                ret->needsRebalancing = false;
                return ret;
            }else{ //Both Black
                auto replacement = left->modify(Black, NULL, NULL);
                auto ret = new Path<Key, Value>(replacement);//
                ret->deleted = this;
                ret->needsRebalancing = true;
                return ret;
            }
        }else{
            if (color == Red){
                auto ret = new Path<Key, Value>();
                ret->deleted = this;
                ret->needsRebalancing = false;
                return ret;
            }else{
                auto ret = new Path<Key, Value>(new RBNode<Key,Value>());
                ret->deleted = this;
                ret->needsRebalancing = true;
                return ret;
            }
        }
    }
    else{ // greater
        if (right == NULL){
            return NULL;
        }else{
            auto partialPath = right->getPath_delete(dKey);
            if (partialPath == NULL)
                return NULL;
            auto ret = partialPath->push(this->modify(left, partialPath->top()), Greater);
            return ret;
        }
    }
}

/*template <class Key, class Value>
Path<Key, Value>* RBNode<Key, Value>::getPath_delete_least(){
    if (left == NULL){
        return remove(val);
    }

    else{
         auto partialPath = left->getPath_delete_least();
         return partialPath->push(this->modify(partialPath->top(), right));  //// may need to change this
    }
        
}
*/
template <class Key, class Value>
Key RBNode<Key, Value>::least(){
    if (left == NULL){
        return key;
    }
    else{
        return left->least();
    }
    
}
template <class Key, class Value>
Key RBNode<Key, Value>::greatest(){
    if (right == NULL){
        return key;
    }
    else{
        return right->greatest();
    }
    
}

template <class Key, class Value>
Value RBNode<Key, Value>::least_val(){
    if (left == NULL){
        return val;
    }
    else{
        return left->least_val();
    }
    
}
template <class Key, class Value>
Value RBNode<Key, Value>::greatest_val(){
    if (right == NULL){
        return val;
    }
    else{
        return right->greatest_val();
    }
    
}

template <class Key, class Value>
Path<Key, Value>* RBNode<Key, Value>::getPath_doubleBlack(RBNode<Key, Value>* node){
    if (node->key < key){ // less
        if (left == NULL){
            return NULL;
        }
        else{
            auto partialPath = left->getPath_doubleBlack(node);
            auto ret = partialPath->push(this->modify(partialPath->top(), right), Less);
            return ret;
        }
    }
    else if(node->key == key){ // equal
        return  new Path<Key, Value> (this->copy());
    }
    else{ // greater
        if (right == NULL){
            return NULL;
        }
        else{
            auto partialPath = right->getPath_doubleBlack(node);
            auto ret = partialPath->push(this->modify(left, partialPath->top()), Greater);
            return ret;
        }
    }
}




/*   Private Funtions   */







#endif /* RBNode_hpp */
