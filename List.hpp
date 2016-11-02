//
//  List.hpp
//  FunctionalDS
//
//  Created by Mermer Dupree on 8/16/16.
//  Copyright © 2016 Mermer Dupree. All rights reserved.
//

#ifndef List_hpp
#define List_hpp

#include <stdio.h>

template <class T> class List{
    List<T> *tail;
    T head;
    int length;
    
public:
    List();
    List(T val, List<T> *rest);
    List(T val);
    List(List<T> *copy, int size);
    List(T h, List<T> *t, int size);
    List<T>* push(T val);
    bool isNull();
    T next();
    List<T>* reverse();
    List<T>* reverse_helper(List<T> *r);
    List<T>* concat(List<T> *c);
    T getHead();
    List<T>* getTail();
    int len();
    static unsigned long long memoryUsed();
    static unsigned long long mem;
};

/***** Function Definitions *****/

// List
template <class T>
unsigned long long List<T>::mem = 0;

template <class T>
List<T>::List(): length(0), head(T()){mem += sizeof(List<T>);};

template <class T>
List<T>::List(T val): head(val), length(1){mem += sizeof(List<T>);};


template <class T>
List<T>::List(T h, List<T> *t, int size): head(h), tail(t),length(size){mem += sizeof(List<T>);}

template <class T>
List<T>::List(List<T> *copy, int size){
    mem += sizeof(List<T>);
    return List<T>(copy->head, copy->tail, size);
}


template <class T>
List<T>::List(T val, List<T> *rest){
    head = val;
    if (rest == NULL){
        length = 1;
        tail = new List<T>();
    }
    else{
        tail = rest;
        length = (rest->length + 1);
    }
    mem += sizeof(List<T>);
};

template <class T>
List<T>* List<T>::push(T val){
    return new List<T>(val, this);
}

template <class T>
T List<T>::next(){
    return head;
}

template <class T>
bool List<T>::isNull(){
    return (length == 0);
}

/*
template <class T>
List<T>* List<T>::reverse(){
    return reverse_helper(new List<T>());
}
*/

template <class T>
List<T>* List<T>::reverse(){
    auto l = this;
    auto ret = new List<T>();
    while (!(l->isNull())){
        ret = ret->push(l->getHead());
        l = l->getTail();
    }
    return ret;
}

template <class T>
List<T>* List<T>::reverse_helper(List<T> *r){
    if (isNull()){
        return r;
    }
    else{
        List<T> *a = new List<T>(head, r);
        return tail->reverse_helper(a);
    }
}
/*
template <class T>
List<T>* List<T>::concat(List<T> *c){
    if (isNull()){
        return c;
    }
    else {
        List<T> *tmp =  (tail->concat(c));
        return new List<T>(head, tmp);
    }
}
*/
template <class T>
List<T>* List<T>::concat(List<T> *c){
    if (isNull()){
        return c;
    }
    List<T>* l = this;
    auto vals = new T[this->length];
    int i = 0;
    
    while(! (l->isNull())){
        vals[i] = l->head;
        i++;
        l = l->getTail();
    }
    auto ret = new List<T>(vals[i - 1], c, c->length + 1);
    for (int i = l->length-2; i >= 0; i--){
        ret = ret->push(vals[i]);
    }
    delete[] vals;
    return ret;
}

template <class T>
T List<T>::getHead(){
    return head;
}

template <class T>
List<T>* List<T>::getTail(){
    if (isNull()) {
        return this;
    }
    else{
        return tail;
    }
}

template <class T>
int List<T>::len(){
    return length;
}

template <class T>
unsigned long long List<T>::memoryUsed(){
    return mem;
}

#endif /* List_hpp */
