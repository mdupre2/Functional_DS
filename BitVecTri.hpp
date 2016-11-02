//
//  BitVecTri.hpp
//  FunctionalDS
//
//  Created by Mermer Dupree on 8/25/16.
//  Copyright © 2016 Mermer Dupree. All rights reserved.
//

#ifndef BitVecTri_hpp
#define BitVecTri_hpp

#include <stdio.h>
#include<iostream>
#include<string>
#include <math.h>
#include <algorithm>
using namespace std;

#define ARR_SIZE 32

/********** BitVecTri ************/
template <class T> class BitVecTri{
private:
    unsigned int size;
    void *arr;
    unsigned int min;
    unsigned int max;
    
    BitVecTri(T val, unsigned int  min, unsigned int max);
    BitVecTri(BitVecTri<T>* first, T val);
    BitVecTri(T val, unsigned int  min, unsigned int  max, unsigned int size);
    BitVecTri(unsigned  min, unsigned int max);
    BitVecTri(unsigned int min, unsigned int max, unsigned int size, T* arr);
    BitVecTri(BitVecTri<T> *bvt);
    
    BitVecTri<T>* append(T val, unsigned int insertPos);
    BitVecTri<T>* shift(BitVecTri<T>* root);
    BitVecTri<T>** branchArr();
    T* leafArr();
    bool isLeaf();
    bool isBranch();
    unsigned int index(unsigned int i);
    BitVecTri<T>* nthBranch(unsigned int n);
    unsigned int branchSize();
    BitVecTri<T>* remove(unsigned int n, BitVecTri<T>* root);
    BitVecTri<T>* update_recurs(T val, unsigned int n);
    
public:
    BitVecTri();
    BitVecTri(T val);
    T nth(unsigned int n);
    BitVecTri<T>* remove(unsigned int n);
    BitVecTri<T>* append(T val);
    T front();
    T last();
    BitVecTri<T>* removeFront();
    BitVecTri<T>* removeLast();
    BitVecTri<T>* update(T val, unsigned int n);
    unsigned int length();
    static unsigned long long mem;
    unsigned long long memoryUsed();
};

template <class T>
unsigned long long BitVecTri<T>::mem = 0;



template <class T>
BitVecTri<T>::BitVecTri(){
    size = 0;
    min = 0;
    max = ARR_SIZE;
    arr = new T[ARR_SIZE];
    mem += ( sizeof(BitVecTri<T>) + (ARR_SIZE * sizeof(T)) );
}

template <class T>
BitVecTri<T>::BitVecTri(BitVecTri<T>* bvt){
    size = bvt->size;
    min = bvt->min;
    max = bvt->max;
    if (max-min <= ARR_SIZE){
        arr = new T[ARR_SIZE];
        for(int i = 0; i < ARR_SIZE; i++){
            ((T*)arr)[i] = ((T*)(bvt->arr))[i];
        }
    }else{
        arr = new BitVecTri<T>*[ARR_SIZE];
        for(int i = 0; i < ARR_SIZE; i++){
            ((BitVecTri<T>**)arr)[i] = ((BitVecTri<T>**)(bvt->arr))[i];
        }
    }
    mem += ( sizeof(BitVecTri<T>) + (ARR_SIZE * sizeof(T)) );
}

template <class T>
BitVecTri<T>::BitVecTri(T val){
    arr = new T[ARR_SIZE];
    ((T*)arr)[0] = val;
    size = 1;
    min = 0;
    max = ARR_SIZE;
    mem += ( sizeof(BitVecTri<T>) + (ARR_SIZE * sizeof(T)) );
}

template <class T>
BitVecTri<T>::BitVecTri(T val, unsigned int min, unsigned int max){
    size = min + 1;
    this->min = min;
    this->max = max;
    int i = (max-min)/ARR_SIZE;
    if (max-min <= ARR_SIZE){
        arr = new T[ARR_SIZE];
        ((T*)arr)[0] = val;
    }else{
        arr = new BitVecTri<T>*[ARR_SIZE];
        ((BitVecTri<T>**)arr)[0] = new BitVecTri<T>(val, min, min+i);
        for (int i = 1; i < ARR_SIZE; i++){
            ((BitVecTri<T>**)arr)[i] = NULL;
        }
    }
    mem += ( sizeof(BitVecTri<T>) + (ARR_SIZE * sizeof(T)) );
}

template <class T>
BitVecTri<T>::BitVecTri(unsigned int min, unsigned int max){
    size = min;
    this->min = min;
    this->max = max;
    if (max-min <= ARR_SIZE){
        arr = new T[ARR_SIZE];
    }else{
        arr = new BitVecTri<T>*[ARR_SIZE];
        for (int i = 0; i < ARR_SIZE; i++){
            ((BitVecTri<T>**)arr)[i] = NULL;
        }
    }
    mem += ( sizeof(BitVecTri<T>) + (ARR_SIZE * sizeof(T)) );
}

template <class T>
BitVecTri<T>::BitVecTri(unsigned int min, unsigned int max, unsigned int size, T *arr){
    this->size = size;
    this->min = min;
    this->max = max;
    (T*)(this->arr) = arr;
    mem += ( sizeof(BitVecTri<T>) + (ARR_SIZE * sizeof(T)) );
}


template <class T>
BitVecTri<T>::BitVecTri(BitVecTri<T>* first, T val){
    size = first->max + 1;
    this->min = first->min;
    this->max = first->max * ARR_SIZE;
    
    arr = new BitVecTri<T>*[ARR_SIZE];
    ((BitVecTri<T>**)arr)[0] = first;
    ((BitVecTri<T>**)arr)[1] = new BitVecTri<T>(val, first->max, (first->max) * 2);
    for (int i = 2; i < ARR_SIZE; i++){
        ((BitVecTri<T>**)arr)[i] = NULL;
    }
    mem += ( sizeof(BitVecTri<T>) + (ARR_SIZE * sizeof(T)) );
}

template <class T>
BitVecTri<T>::BitVecTri(T val, unsigned int min, unsigned int max, unsigned int size){
    this->size = size;
    this->min = min;
    this->max = max;
    int i = (max-min)/ARR_SIZE;
    if (max-min <= ARR_SIZE){
        arr = new T[ARR_SIZE];
        ((T*)arr)[0] = val;
    }else{
        arr = new BitVecTri<T>*[ARR_SIZE];
        ((BitVecTri<T>**)arr)[0] = new BitVecTri<T>(val, min, i);
        for (int i = 1; i < ARR_SIZE; i++){
            ((BitVecTri<T>**)arr)[i] = NULL;
        }
    }
    mem += ( sizeof(BitVecTri<T>) + (ARR_SIZE * sizeof(T)) );
}

template <class T>
unsigned long long BitVecTri<T>::memoryUsed(){
    return mem;
}

template <class T>
BitVecTri<T>** BitVecTri<T>::branchArr(){
    return ((BitVecTri<T>**)arr);
}

template <class T>
T* BitVecTri<T>::leafArr(){
    return ((T*)arr);
}

template <class T>
bool BitVecTri<T>::isLeaf(){
    return (max-min == ARR_SIZE);
}

template <class T>
bool BitVecTri<T>::isBranch(){
    return (max-min > ARR_SIZE);
}

template <class T>
unsigned int BitVecTri<T>::index(unsigned int i){
    int j = (max-min)/ARR_SIZE;
    int k = (i-min)/j;
    return k;
}

template <class T>
BitVecTri<T>* BitVecTri<T>::nthBranch(unsigned int n){
    return (((BitVecTri<T>**)arr)[n]);
}

template <class T>
unsigned int BitVecTri<T>::branchSize(){
    return (max-min)/ARR_SIZE;
}

template <class T>
T BitVecTri<T>::nth(unsigned int n){
    if (isLeaf()){
        return leafArr()[index(n)];
    }else{
        return branchArr()[index(n)]->nth(n);
    }
}

template <class T>
unsigned int BitVecTri<T>::length(){
    return size;
}

template <class T>
T BitVecTri<T>::front(){
    return nth(0);
}

template <class T>
T BitVecTri<T>::last(){
    return nth(size-1);
}

template <class T>
BitVecTri<T>* BitVecTri<T>::removeFront(){
    return remove(0);
}

template <class T>
BitVecTri<T>* BitVecTri<T>::removeLast(){
    return remove(size-1);
}

template <class T>
BitVecTri<T>* BitVecTri<T>::update(T val, unsigned int n){
    if(n >= size){
        return this;
    }else{
        return update_recurs(val, n);
    }
}

template <class T>
BitVecTri<T>* BitVecTri<T>::update_recurs(T val, unsigned int n){
    auto ret = new BitVecTri<T>(this);
    if (isLeaf()){
        ret->leafArr()[index(n)] = val;
    }else{
        ret->branchArr()[index(n)] = (branchArr()[index(n)])->update(val, n);
    }
    return ret;
}

template <class T>
BitVecTri<T>* BitVecTri<T>::remove(unsigned int n){
    if (n >= size){
        return this;
    }
    else if (size == 1){
        return new BitVecTri<T>();
    }
    else{
        if (isBranch()){
            if (n == size-1){ // if removing last
                if (size == branchSize() + 1){ // if need to remove this branch because not enough elements after removal
                    auto ret = branchArr()[0];
                    ret->size = size-1;
                    return ret;
                }
                else{
                    return remove(n, this);
                    auto ret = new BitVecTri<T>(this);
                    ret->size = size - 1;
                    return ret;
                }
            }
            else{
                if (size == branchSize() + 1){ // if need to remove this branch because not enough elements after removal
                    return (branchArr()[0])->remove(n,this);
                }
                else{
                    return remove(n,this);
                }
            }
        }
        else{ // is leaf
            return remove(n, this);
        }
    }
}

template <class T>
BitVecTri<T>* BitVecTri<T>::remove(unsigned int n, BitVecTri<T>* root){
    if(isLeaf()){
        auto ret =  new BitVecTri<T>(min, max);
        ret->size = root->size - 1;
        for (int i = min; i < n; i++){
            ret->leafArr()[index(i)] = root->nth(i);
        }
        for (int i = n+1; (i < max+1 && i < root->size); i++){
            ret->leafArr()[index(i-1)] = root->nth(i);
        }
        return ret;
    }else{
        auto ret = new BitVecTri<T>(this);
        ret->size = root->size-1;
        (ret->branchArr()[index(n)]) = (branchArr()[index(n)])->remove(n, root);
        for(int i = index(n) + 1; i < ARR_SIZE; i++){
            if (ret->branchArr()[i] != NULL){
                ret->branchArr()[i] = branchArr()[i]->shift(root);
            }
        }
        return ret;
    }
}

template <class T>
BitVecTri<T>* BitVecTri<T>::shift(BitVecTri<T>* root){
    if (root->size-1 == min){
        return NULL;
    }
    if (isLeaf()){
        auto ret =  new BitVecTri<T>(min, max);
        ret->size = root->size - 1;
        for (int i = min; (i < max && i < (root->size-1)); i++){
            ret->leafArr()[index(i)] = root->nth(i+1);
        }
        return ret;
    }else{
        auto ret =  new BitVecTri<T>(min, max);
        ret->size = root->size-1;
        for(int i = 0; i < ARR_SIZE; i++){
            if (branchArr()[i] != NULL){
                ret->branchArr()[i] = branchArr()[i]->shift(root);
            }
        }
        return ret;
    }
}

template <class T>
BitVecTri<T>* BitVecTri<T>::append(T val){
    if (size == 0){
        return new BitVecTri<T>(val);
    }
    else if (size == max){
        return new BitVecTri<T>(this, val);
    }else{
        return append(val, size);
    }
}

template <class T>
BitVecTri<T>* BitVecTri<T>::append(T val, unsigned int insertPositon){
    BitVecTri<T>* ret;
    if (isLeaf()){
        ret = new BitVecTri<T>(this);
        ret->size = size + 1;
        ret->leafArr()[index(size)] = val;
        return ret;
    }else{
        int leavesPerBranch = branchSize(); //number of items in each BitVecTri
        int branchNum = (insertPositon-min)/leavesPerBranch; // index of BitVecTri to insert val
        BitVecTri<T>* branch = nthBranch(branchNum);
        if (branch != NULL){
            ret = new BitVecTri<T>(this);
            ret->branchArr()[branchNum] = branch->append(val, insertPositon);
            ret->size = insertPositon+1;
            return ret;
        }else{
            ret = new BitVecTri<T>(this);
            BitVecTri<T>* tmp = new BitVecTri<T>(insertPositon, insertPositon+leavesPerBranch);
            ret->branchArr()[branchNum] = tmp->append(val, insertPositon);
            ret->size = insertPositon+1;
            return ret;
        }
    }
}

#endif /* BitVecTri_hpp */
