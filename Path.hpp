//
//  Path.hpp
//  FunctionalDS
//
//  Created by Mermer Dupree on 8/18/16.
//  Copyright © 2016 Mermer Dupree. All rights reserved.
//

#ifndef Path_hpp
#define Path_hpp

#include <stdio.h>
#include <iostream>
#include "List.hpp"
#include "enums.hpp"

template <class Key, class Value> class RBNode;


/********** Path ************/

template <class Key, class Value> class Path {
public:
    //ChildType childType;
    List<RBNode<Key, Value>*>* pathNodes;
    List<Comparison>* directions;
    RBNode<Key, Value>* greatgrandparent;
    RBNode<Key, Value>* grandparent;
    RBNode<Key, Value>* parent;
    RBNode<Key, Value>* uncle;
    RBNode<Key, Value>* child;
    RBNode<Key, Value>* sibling;
    RBNode<Key, Value>* deleted;
    
    bool needsRebalancing = false;
    Comparison greatgrandparentComparison;
    Comparison grandparentComparison;
    Comparison parentComparison;
    static unsigned long long mem;
    
    
    Path();
    Path(List<RBNode<Key, Value>*>* pathNodes, List<Comparison>* directions);
    Path(RBNode<Key, Value>* pathNode, Comparison comparison);
    Path(RBNode<Key, Value>* pathNode);
    ChildType childType();
    bool isLeftChild();
    Color uncleColor();
    Color siblingColor();
    bool hasRedNephew();
    bool leftNephewRed();
    bool rightNephewRed();
    Path<Key, Value>* push(RBNode<Key, Value>*);
    Path<Key, Value>* push(RBNode<Key, Value>*, Comparison comparison);
    RBNode<Key, Value>* top();
    void print();
    int nNodes();
    bool isEmpty();
private:
    void print_recursive(List<RBNode<Key, Value>*>* pn);
};

/********** Constructor ************/
template <class Key, class Value>
unsigned long long Path<Key, Value>::mem = 0;

template <class Key, class Value>
Path<Key, Value>::Path(){
    this->pathNodes = new List<RBNode<Key, Value>*>();
    this->directions = new List<Comparison>();
    this->grandparent = NULL;
    this->parent = NULL;
    this->uncle = NULL;
    this->child = NULL;
    this->sibling = NULL;
    this->greatgrandparent = NULL;
    this->deleted = NULL;
    mem += sizeof(Path<Key,Value>);
}

template <class Key, class Value>
Path<Key, Value>::Path(List<RBNode<Key, Value>*>* pathNodes, List<Comparison>* directions){
    this->pathNodes = pathNodes;
    this->directions = directions;
    this->grandparent = NULL;
    this->parent = NULL;
    this->uncle = NULL;
    this->child = NULL;
    this->sibling = NULL;
    this->greatgrandparent = NULL;
    this->deleted = NULL;
    mem += sizeof(Path<Key,Value>);
}

template <class Key, class Value>
Path<Key, Value>::Path(RBNode<Key, Value>* pathNode, Comparison comparison){
    this->pathNodes = new List<RBNode<Key,Value>*>(pathNode);
    this->directions = new List<Comparison>(comparison);
    this->grandparent = NULL;
    this->parent = NULL;
    this->uncle = NULL;
    this->greatgrandparent = NULL;
    this->child = pathNode;
    this->sibling = NULL;
    this->deleted = NULL;
    mem += sizeof(Path<Key,Value>);
}

template <class Key, class Value>
Path<Key, Value>::Path(RBNode<Key, Value>* pathNode){
    this->pathNodes = new List<RBNode<Key,Value>*>(pathNode);
    this->directions = new List<Comparison>();
    this->grandparent = NULL;
    this->parent = NULL;
    this->uncle = NULL;
    this->greatgrandparent = NULL;
    this->sibling = NULL;
    this->child = pathNode;
    this->deleted = NULL;
    mem += sizeof(Path<Key,Value>);
}

/********** Methods ************/
// only call if there are at least  2 in directions
template <class Key, class Value>
ChildType Path<Key, Value>::childType(){
    //Comparison second = directions->getHead();
    //Comparison first = directions->getTail()->getHead();
    if (parent == NULL || grandparent == NULL){
        std::cout << "Error with ChildType\n";
        return RightRight;
    }
    
    if (grandparentComparison == Greater){
        if (parentComparison == Greater){
            return RightRight;
        }
        else{
            return RightLeft;
        }
    }
    else{
        if (parentComparison == Greater){
            return LeftRight;
        }
        else{
            return LeftLeft;
        }
    }
}

template <class Key, class Value>
bool Path<Key, Value>::isLeftChild(){
        if (parentComparison == Greater){
            return false;
        }
        else{
            return true;
        }
}


// only call if there are at least three nodes in pathNodes
// and at least 2 in directions
template <class Key, class Value>
Color Path<Key, Value>::uncleColor(){
    if (uncle == NULL){
        return Black;
    }
    return uncle->color;
}

template <class Key, class Value>
Color Path<Key, Value>::siblingColor(){
    if (sibling == NULL){
        return Black;
    }
    return sibling->color;
}

// push //
template <class Key, class Value>
Path<Key, Value>* Path<Key, Value>::push(RBNode<Key, Value>* pathNode){
    auto ret = new Path<Key, Value>(pathNodes->push(pathNode), directions );
    ret->deleted = deleted;
    ret->needsRebalancing = needsRebalancing;
    if(ret->pathNodes->len() == 4){
        ret->greatgrandparent = ret->pathNodes->getHead();
    }else{
        ret->greatgrandparent = greatgrandparent;
        ret->greatgrandparentComparison = greatgrandparentComparison;
    }
    if(ret->pathNodes->len() == 3){
        ret->grandparent = ret->pathNodes->getHead();
    }else{
        ret->grandparent = grandparent;
    }
    if(ret->pathNodes->len() == 2){
        ret->parent = ret->pathNodes->getHead();
        
    }else{
        ret->parent = parent;
    }
    if(ret->pathNodes->len() == 1){
        ret->child = ret->pathNodes->getHead();
    }else{
        ret->child = child;
    }
    return ret;
}

// push //
template <class Key, class Value>
Path<Key, Value>* Path<Key, Value>::push(RBNode<Key, Value>* pathNode, Comparison comparison){
    auto ret =  new Path<Key, Value>(pathNodes->push(pathNode), directions->push(comparison) );
    ret->deleted = deleted;
    ret->needsRebalancing = needsRebalancing;
    if(ret->pathNodes->len() == 4){
        ret->greatgrandparent = ret->pathNodes->getHead();
        ret->greatgrandparentComparison = comparison;
    }else{
        ret->greatgrandparent = greatgrandparent;
        ret->greatgrandparentComparison = greatgrandparentComparison;
    }
    if(ret->pathNodes->len() == 3){
        ret->grandparent = ret->pathNodes->getHead();
        ret->grandparentComparison = comparison;
        if (comparison == Greater){
            ret->uncle = ret->grandparent->left;
        }else{
            ret->uncle = ret->grandparent->right;
        }
    }else{
        ret->grandparent = grandparent;
        ret->uncle = uncle;
        ret->grandparentComparison = grandparentComparison;
    }
    if(ret->pathNodes->len() == 2){
        ret->parent = ret->pathNodes->getHead();
        ret->parentComparison = comparison;
        if (comparison == Greater){
            ret->sibling = ret->parent->left;
        }else{
            ret->sibling = ret->parent->right;
        }
    }else{
        ret->parent = parent;
        ret->parentComparison = parentComparison;
        ret->sibling = sibling;
    }
    if(ret->pathNodes->len() == 1){
        ret->child = ret->pathNodes->getHead();
    }else{
        ret->child = child;
    }
    return ret;
}

template <class Key, class Value>
RBNode<Key, Value>* Path<Key, Value>::top(){
    if (isEmpty())
        return NULL;
    return pathNodes->getHead();
}

template <class Key, class Value>
void Path<Key, Value>::print(){
    print_recursive(pathNodes);
}

template <class Key, class Value>
void Path<Key, Value>::print_recursive( List<RBNode<Key, Value>*>* pn){
    if (pn != NULL){
        pn->getHead()->print();
        print_recursive(pn->getTail());
    }
}

template <class Key, class Value>
int Path<Key, Value>::nNodes(){
    return pathNodes->len();
}


template <class Key, class Value>
bool Path<Key, Value>::isEmpty(){
    return pathNodes->len() == 0;
}

template <class Key, class Value>
bool Path<Key, Value>::hasRedNephew(){
    if (sibling != NULL){
        if (sibling->left != NULL && sibling->left->color == Red){
            return true;
        }
        if (sibling->right != NULL && sibling->right->color == Red){
            return true;
        }
    }
    return false;
}

template <class Key, class Value>
bool Path<Key, Value>::leftNephewRed(){
    if (sibling != NULL){
        if (sibling->left != NULL && sibling->left->color == Red){
            return true;
        }
    }
    return false;
}

template <class Key, class Value>
bool Path<Key, Value>::rightNephewRed(){
    if (sibling != NULL){
        if (sibling->right != NULL && sibling->right->color == Red){
            return true;
        }
    }
    return false;
}

#endif /* Path_hpp */
