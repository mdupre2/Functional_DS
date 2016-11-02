//
//  FQueue.hpp
//  FunctionalDS
//


#ifndef FQueue_hpp
#define FQueue_hpp

#include <stdio.h>

#include "List.hpp"


template <class T> class FQueue{
    List<T>* front;
    List<T>* back;
    int length;
public:
    FQueue();
    FQueue(List<T>* f, List<T>* b);
    FQueue<T>* enqueue(T val);
    FQueue<T>* dequeue();
    T next();
    List<T>* getFront();
    List<T>* getBack();
    bool isEmpty();
};

// FQueue

template <class T>
FQueue<T>::FQueue(){
    front = new List<T>();
    back = new List<T>();
    length = 0;
};

template <class T>
FQueue<T>::FQueue(List<T>* f, List<T>* b): front(f), back(b), length(b->len() + f->len()){}

template <class T>
FQueue<T>* FQueue<T>::enqueue(T val){
    if (back->len() + 1 > front->len()){
        return new FQueue(front->concat(back->push(val)->reverse()), new List<T>());
    }
    else{
        return new FQueue(front, back->push(val));
    }
}

template <class T>
FQueue<T>* FQueue<T>::dequeue(){
    if (front->len() - 1 < back->len()){
        return new FQueue(front->getTail()->concat(back->reverse()), new List<T>());
    }
    else{
        return new FQueue(front->getTail(), back);
    }
}

template <class T>
T FQueue<T>::next(){
    return front->head();
}

template <class T>
List<T>* FQueue<T>::getFront(){
    return front;
}

template <class T>
List<T>* FQueue<T>::getBack(){
    return back;
}

template <class T>
bool FQueue<T>::isEmpty(){
    return (length == 0);
}







#endif /* FQueue_hpp */
