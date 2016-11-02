//
//  BankersQueue.hpp
//  FunctionalDS
//
//  Created by Mermer Dupree on 10/25/16.
//  Copyright © 2016 Mermer Dupree. All rights reserved.
//

#ifndef BankersQueue_hpp
#define BankersQueue_hpp

#include <stdio.h>

#include "List.hpp"


template <class T> class BankersQueue{
    List<T>* front;
    List<T>* back;
    int length;
public:
    BankersQueue();
    BankersQueue(List<T>* f, List<T>* b);
    BankersQueue<T>* enqueue(T val);
    BankersQueue<T>* dequeue();
    T next();
    List<T>* getFront();
    List<T>* getBack();
    bool isEmpty();
    static unsigned long long mem;
    static unsigned long long memoryUsed();
};

// BankersQueue
template <class T>
unsigned long long BankersQueue<T>::mem = 0;

template <class T>
BankersQueue<T>::BankersQueue(){
    front = new List<T>();
    back = new List<T>();
    length = 0;
    mem += sizeof(BankersQueue<T>);
};

template <class T>
BankersQueue<T>::BankersQueue(List<T>* f, List<T>* b): front(f), back(b), length(b->len() + f->len()){mem += sizeof(BankersQueue<T>);}

template <class T>
BankersQueue<T>* BankersQueue<T>::enqueue(T val){
    if (back->len() + 1 > front->len()){
        return new BankersQueue(front->concat((back->push(val))->reverse()), new List<T>());
    }
    else{
        return new BankersQueue(front, back->push(val));
    }
}

template <class T>
BankersQueue<T>* BankersQueue<T>::dequeue(){
    if (front->len() - 1 < back->len()){
        return new BankersQueue(front->getTail()->concat(back->reverse()), new List<T>());
    }
    else{
        return new BankersQueue(front->getTail(), back);
    }
}

template <class T>
T BankersQueue<T>::next(){
    return front->head();
}

template <class T>
List<T>* BankersQueue<T>::getFront(){
    return front;
}

template <class T>
List<T>* BankersQueue<T>::getBack(){
    return back;
}

template <class T>
bool BankersQueue<T>::isEmpty(){
    return (length == 0);
}

template <class T>
unsigned long long BankersQueue<T>::memoryUsed(){
    return mem + List<T>::mem;
}




#endif /* BankersQueue_hpp */
