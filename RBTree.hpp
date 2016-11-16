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
#include <queue>
#include <math.h>
#include <fstream>
#include <cstdlib>

using namespace std;

extern string to_String(Color c);
extern string to_String(Comparison c);


/********** RBTree ************/
template <class Key, class Value> class RBTree{
private:
    
    
public:
    RBNode<Key, Value>* root;
    
    Path<Key,Value>* path;
    
    RBTree();
    RBTree(Key key, Value val);
    RBTree(RBNode<Key, Value>* root);
    RBTree<Key, Value>* insert(Key key, Value val);
    RBTree<Key, Value>* insert(RBNode<Key,Value>* node);
    RBTree<Key, Value>* remove(Key key);
    Path<Key, Value>* doubleBlack(Path<Key,Value>* path);
    Value get(Key key);
    bool hasKey(Key key);
    
    int blackHeight();
    bool hasRedParentChild();
    bool isValid();
    void print();
    void bfprint(int level);
    string dotfile();
    string dotfile(string title);
    void image(string filename);
    Value front();
    Value last();
    RBTree<Key, Value>* removeFront();
    RBTree<Key, Value>* removeLast();
    unsigned long long memoryUsed();
    static unsigned long long mem;
};

/*   Public Funtions   */

template <class Key, class Value>
unsigned long long RBTree<Key, Value>::mem = 0;

template <class Key, class Value>
RBTree<Key, Value>::RBTree(){
    root = NULL;
    mem += sizeof(RBTree<Key, Value>);
}

template <class Key, class Value>
RBTree<Key, Value>::RBTree(Key key, Value val){
    root = new RBNode<Key, Value>(Black, key, val, NULL, NULL, 1);
    mem += sizeof(RBTree<Key, Value>);
}

template <class Key, class Value>
RBTree<Key, Value>::RBTree(RBNode<Key, Value>* root){
    this->root = root;
    mem += sizeof(RBTree<Key, Value>);
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
unsigned long long RBTree<Key, Value>::memoryUsed(){
    return RBNode<Key, Value>::mem + RBTree<Key, Value>::mem + Path<Key,Value>::mem + List<RBNode<Key, Value>*>::mem;
}

template <class Key, class Value>
Value RBTree<Key, Value>::front(){
    if ( root == NULL){
        return Value();
    }
    return root->least_val();
}

template <class Key, class Value>
Value RBTree<Key, Value>::last(){
    if ( root == NULL){
        return Value();
    }
    return root->greatest_val();
}

template <class Key, class Value>
RBTree<Key, Value>* RBTree<Key, Value>::removeFront(){
    if ( root == NULL){
        return this;
    }
    return root->remove(root->least());
}

template <class Key, class Value>
RBTree<Key, Value>* RBTree<Key, Value>::removeLast(){
    if ( root == NULL){
        return this;
    }
    return root->remove(root->greatest());
}

template <class Key, class Value>
void RBTree<Key, Value>::image(string filename){
    string dot = dotfile(filename);
    string fn =filename + ".dot";
    std::ofstream os(fn);
    os << dot;
    os.close();
    string cmd = "/usr/local/bin/dot ";
    cmd += fn;
    cmd += " -Tpng > ";
    cmd += filename;
    cmd += ".png";
    std::system(cmd.c_str());
}


template <class Key, class Value>
string RBTree<Key, Value>::dotfile(){
    string s;
    s += "digraph g{\n";
    if (root != NULL){
        s += root->dotfile_recurs("r");
    }
    else{
        s +=  s += "r[label=\"NULL\", shape=rectangle, style=filled, fillcolor=black, fontcolor=white]\n";
    }
    s += "\n}\n";
    return s;
}

template <class Key, class Value>
string RBTree<Key, Value>::dotfile(string title){
    string s;
    s += "digraph g{\n";
    s += "label=\"";
    s += title;
    s += "\"\n";
    s += "labelloc=top;\nlabeljust=left;\n";
    if (root != NULL){
        s += root->dotfile_recurs("r");
    }
    else{
        s +=  s += "r[label=\"NULL\", shape=rectangle, style=filled, fillcolor=black, fontcolor=white]\n";
    }
    s += "\n}\n";
    return s;
}

template <class Key, class Value>
int RBTree<Key, Value>::blackHeight(){
    if (root == NULL){
        return 0;
    }else{
        return root->blackHeight();
    }
}

template <class Key, class Value>
bool RBTree<Key, Value>::hasRedParentChild(){
    if (root == NULL){
        return false;
    }else{
        return root->hasRedParentChild();
    }
}

template <class Key, class Value>
bool RBTree<Key, Value>::isValid(){
    bool ret1, ret2;
    if (blackHeight() == -1){
        cout << "Error: Has invalid black height" << endl;
        ret1 = false;
    }else{
        ret1 = true;
    }
    if (hasRedParentChild()){
        cout << "Error: Has red parent and child" << endl;
        ret2 = false;
    }else{
        ret2 = true;
    }
    return (ret1 && ret2);
}

template <class Key, class Value>
Value RBTree<Key, Value>::get(Key key){
    Value v;
    
    if (root != NULL){
        RBNode<Key, Value>* r = root->search(key);
        if (r != NULL){
            v = r->val;
        }else{
            cout << "Not found" << endl;
        }
    }else{
        cout << "not found" << endl;
    }
    return v;
}

template <class Key, class Value>
bool RBTree<Key, Value>::hasKey(Key key){
    
    if (root != NULL){
        auto r = root->get(key);
        if (r != NULL){
            return true;
        }
    }
    return false;
}

template <class Key, class Value>
void RBTree<Key, Value>::bfprint(int levels){
    std::queue<RBNode<Key,Value>*> nodeQueue;
    int power = 0;
    int i = 1;
    if(root != NULL){
        nodeQueue.push(root);
    }
    cout << "******" << endl;
    while(!nodeQueue.empty() && power < levels){
        if( i == 0){
            cout << "******" << endl;
            power++;
            i = pow(2,power);
            
        }
        RBNode<Key,Value>* cur = nodeQueue.front();
        nodeQueue.pop();
        if (cur == NULL){
            cout << "\nNULL" << endl;
            nodeQueue.push(NULL);
            nodeQueue.push(NULL);
        }else{
            nodeQueue.push(cur->left);
            nodeQueue.push(cur->right);
            cout << "\n";
            cur->print();
            //print(cur);
        }
        i--;
    }
}

//  insert  //
template <class Key, class Value>
RBTree<Key, Value>* RBTree<Key, Value>::insert(Key key, Value val){
    return insert(new RBNode<Key,Value>(Red, key, val, NULL, NULL, 1));
    if (root == NULL){
        return new RBTree<Key,Value>(key, val);
    }
    Path<Key,Value>* path = root->getPath(key, val);
    if (path->needsRebalancing){
        if (path->childType() == RightRight){
            auto tmp = path->parent->left;
            path->parent->left = path->grandparent;
            path->grandparent->right = tmp;
            if (path->greatgrandparent != NULL){
                if (path->greatgrandparentComparison == Greater){
                    path->greatgrandparent->right = path->parent;
                }else{
                    path->greatgrandparent->left = path->parent;
                }
            }else{
                return new RBTree<Key,Value> (path->parent);
            }
        }
    }
    auto ret =  new RBTree<Key,Value> (path->top());
    ret->path = path;
    return ret;
}

//  insert  //
template <class Key, class Value>
RBTree<Key, Value>* RBTree<Key, Value>::insert(RBNode<Key,Value>* node){
    /* empty tree */
    if (root == NULL){
        if (node->color == Red){
            return new RBTree<Key,Value>(node->recolor(Black));
        }else{
            return new RBTree<Key,Value>(node);
        }
        //return node;
    }
    
    /* NonEmpty insert */
    Path<Key,Value>* path = root->getPath(node);
    RBTree<Key,Value>* ret;
    //if (path->nNodes() >= 3 && path->parent != NULL && path->parent->color == Red){ // if there is a violation
    if (path->needsRebalancing){
        /* Black Uncle */
        if (path->uncleColor() == Black ){
            
            /* Case 3A */
            if (path->childType() == RightRight){
                /* Swap Color */
                Color tmpColor = path->grandparent->color;
                path->grandparent->color = path->parent->color;
                path->parent->color = tmpColor;
                /* Left Rotate around Grandparent */
                auto tmp = path->parent->left;
                path->parent->left = path->grandparent;
                path->grandparent->right = tmp;
                if (path->greatgrandparent != NULL){
                    if (path->greatgrandparentComparison == Greater){
                        path->greatgrandparent->right = path->parent;
                        ret = new RBTree<Key,Value> (path->top());
                        //ret =  (new RBTree<Key,Value> (path->top(), 1))->insert(path->parent);//new RBTree<Key,Value> (path->top(), 1);
                    }else{
                        path->greatgrandparent->left = path->parent;
                        //ret =  (new RBTree<Key,Value> (path->top(), 1))->insert(path->parent);
                        ret = new RBTree<Key,Value> (path->top());
                    }
                }else{
                    path->parent->color = Black;
                    ret = new RBTree<Key,Value> (path->parent);
                }
            }
            /* End of Case 3A */
            
            /* Case 3B */
            else if (path->childType() == LeftLeft){
                /* Swap Color */
                Color tmpColor = path->grandparent->color;
                path->grandparent->color = path->parent->color;
                path->parent->color = tmpColor;
                /* Right Rotate around Grandparent */
                auto tmp = path->parent->right;
                path->parent->right = path->grandparent;
                path->grandparent->left= tmp;
                if (path->greatgrandparent != NULL){
                    if (path->greatgrandparentComparison == Greater){
                        path->greatgrandparent->right = path->parent;
                        ret = new RBTree<Key,Value> (path->top());
                        //ret =  (new RBTree<Key,Value> (path->top(), 1))->insert(path->parent);//new RBTree<Key,Value> (path->top(), 1);
                    }else{
                        path->greatgrandparent->left = path->parent;
                        //ret =  (new RBTree<Key,Value> (path->top(), 1))->insert(path->parent);
                        ret = new RBTree<Key,Value> (path->top());
                    }
                }else{
                    path->parent->color = Black;
                    ret = new RBTree<Key,Value> (path->parent);
                }
            }
            /* End of Case 3B */
            
            /* Case 2A */
            else if (path->childType() == LeftRight){
                path->grandparent->left = path->child;
                path->parent->right = path->child->left;
                path->child->left = NULL;
                ret =  (new RBTree<Key,Value> (path->top()))->insert(path->parent);
            }
            /* End of Case 2A */
            
            /* Case 2B */
            else if (path->childType() == RightLeft){
                path->grandparent->right = path->child;
                path->parent->left = path->child->right;
                path->child->right = NULL;
                ret =  (new RBTree<Key,Value> (path->top()))->insert(path->parent);
            }
            /* End of Case 2B */
            
        }
        /* Red Uncle */
        /* Case 1 */
        else if (path->uncleColor() == Red){
            path->uncle->color = Black;
            path->parent->color = Black;
            path->grandparent->color = Red;
            if (path->greatgrandparent != NULL){
                if (path->greatgrandparentComparison == Greater){
                    path->greatgrandparent->right = NULL;
                    ret =  (new RBTree<Key,Value> (path->top()))->insert(path->grandparent);
                }else{
                    path->greatgrandparent->left = NULL;
                    ret =  (new RBTree<Key,Value> (path->top()))->insert(path->grandparent);
                }
            }else{
                path->grandparent->color = Black;
                ret = new RBTree<Key,Value> (path->top());
            }
            
        }
    }else{ // No violation
        ret =  new RBTree<Key,Value> (path->top());
    }
    ret->path = path;
    return ret;
}

//   Remove   //
template <class Key, class Value>
RBTree<Key, Value>* RBTree<Key, Value>::remove(Key key){
    if (root == NULL){
        std::cout << "Element not it RBTree. Tree is empty." << std::endl;
        return this;
    }
    if (root->isLeaf() ){
        if (root->key == key){
            return new RBTree<Key,Value>();
        }else{
            std::cout << "Element not it RBTree" << std::endl;
            std::cout << "key: " << key << " ,root: " << root->key << std::endl;
            return this;
        }
        
    }
    auto deletePath = root->getPath_delete(key);
    if (deletePath == NULL){
        std::cout << "Element not it RBTree 1" << std::endl;
        return this;
    }
    if (deletePath->needsRebalancing){
        //image("predelete"); // Debug Code
        //std::cout << "Needed Rebalancing" << std::endl; // Debug Code
        auto tmp =  new RBTree<Key,Value> (deletePath->top());
        //tmp->image("prebalance"); // Debug Code
        auto ret =  new RBTree<Key,Value> (doubleBlack(deletePath)->top());
        //ret->image("postbalance"); // Debug Code
        return ret;
    }else{
        //std::cout << "Did Not Need Rebalancing" << std::endl; // Debug Code
        return new RBTree<Key,Value>(deletePath->top());
    }
}

template <class Key, class Value>
Path<Key, Value>* RBTree<Key, Value>::doubleBlack(Path<Key,Value>* path){
    
    if (path == NULL){
        return NULL;
    }
    
    if (path->child->key == root->key || path->parent == NULL){
        if (path->child->color != Black){
            path->child->color = Black;
        }
        return path;
    }
    if (path->sibling->color == Black && path->hasRedNephew()){// add check for sibling
        if ( path->isLeftChild() && path->rightNephewRed() ){
            auto sibling = path->sibling->recolor(path->parent->color);
            path->parent->color = Black;
            sibling->right = sibling->right->recolor(Black);
            path->parent->right = sibling->left;
            sibling->left = path->parent;
            if (path->parent->left != NULL && path->parent->left->isNull){
                path->parent->left = NULL;
            }
            if (path->grandparent != NULL){
                if (path->grandparentComparison == Greater){
                    path->grandparent->right = sibling;
                }else{
                    path->grandparent->left = sibling;
                }
                return path;
            }else{
                return new Path<Key, Value>(sibling);
            }
            
        }
        else if ( !(path->isLeftChild()) && path->leftNephewRed() ){
            auto sibling = path->sibling->recolor(path->parent->color);
            path->parent->color = Black;
            sibling->left = sibling->left->recolor(Black);
            path->parent->left = sibling->right;
            sibling->right = path->parent;
            if (path->parent->right != NULL && path->parent->right->isNull){
                path->parent->right = NULL;
            }
            if (path->grandparent != NULL){
                if (path->grandparentComparison == Greater){
                    path->grandparent->right = sibling;
                }else{
                    path->grandparent->left = sibling;
                }
                return path;
            }
            else{
                return new Path<Key, Value>(sibling);
            }
        
        }
        //
        else if ( path->isLeftChild() && path->leftNephewRed() ){
            auto sibling = path->sibling->recolor(Red);
            auto sibling_left = sibling->left->recolor(Black);
            auto sibling_left_right = sibling_left->right;
            sibling_left->right = sibling;
            sibling->left = sibling_left_right ;
            path->parent->right = sibling_left;
            //
            if (path->parent->right != NULL){
                sibling = path->parent->right->recolor(path->parent->color);
            }else{
                sibling = path->parent->right;
            }
            path->parent->color = Black;//
            sibling->right = sibling->right->recolor(Black);
            path->parent->right = sibling->left;
            sibling->left = path->parent;
            if (path->parent->left != NULL && path->parent->left->isNull){
                path->parent->left = NULL;
            }
            if (path->grandparent != NULL){
                if (path->grandparentComparison == Greater){
                    path->grandparent->right = sibling;
                }else{
                    path->grandparent->left = sibling;
                }
                return path;
            }
            else{
                return new Path<Key, Value>(sibling);
            }
            
        }
        else if ( !(path->isLeftChild()) && path->rightNephewRed() ){
            auto sibling = path->sibling->recolor(Red);
            auto sibling_right = sibling->right->recolor(Black);
            auto sibling_right_left = sibling_right->left;
            sibling_right->left = sibling;
            sibling->right = sibling_right_left ;
            path->parent->left = sibling_right;
            //
            if (path->parent->left != NULL){
                sibling = path->parent->left->recolor(path->parent->color);
            }else{
                sibling = path->parent->left;
            }
            path->parent->color = Black;
            sibling->left = sibling->left->recolor(Black);
            path->parent->left = sibling->right;
            sibling->right = path->parent;
            if (path->parent->right != NULL && path->parent->right->isNull){
                path->parent->right = NULL;
            }
            if (path->grandparent != NULL){
                if (path->grandparentComparison == Greater){
                    path->grandparent->right = sibling;
                }else{
                    path->grandparent->left = sibling;
                }
                return path;
            }
            else{
                return new Path<Key, Value>(sibling);
            }
            
        }
    }
    else if (path->sibling->color == Black && !(path->hasRedNephew()) ){ // case 2
        if (path->isLeftChild()){
            path->parent->right = path->parent->right->recolor(Red);
            if (path->parent->left != NULL && path->parent->left->isNull){
                path->parent->left = NULL;
            }
        }
        else{
            path->parent->left = path->parent->left->recolor(Red);
            if (path->parent->right != NULL && path->parent->right->isNull){
                path->parent->right = NULL;
            }
        }
        if (path->parent->color == Red){
            path->parent->color = Black;
            return path;
        }
        else{
            if (path->top() != NULL){
                return doubleBlack(path->top()->getPath_doubleBlack(path->parent));
            }else{
                return path;
            }
        }
    }
    else if (path->sibling->color == Red){
        if (path->isLeftChild()){
            auto sibling = path->sibling->recolor(Black);
            auto parent = path->parent->recolor(Red);
            auto sibling_left = sibling->left;
            sibling->left = parent;
            parent->right = sibling_left;
            
            if (path->grandparent != NULL){
                if (path->grandparentComparison == Greater){
                    path->grandparent->right = sibling;
                }else{
                    path->grandparent->left = sibling;
                }
                path->parent = parent;
                path->grandparentComparison = Less;
                path->grandparent = sibling;
                path->sibling = path->parent->right;
                return doubleBlack(path);
            }else{
                auto newPath = new Path<Key, Value>(path->child);
                newPath = newPath->push(parent,Less);
                newPath = newPath->push(sibling, Less);
                return doubleBlack(newPath);
            }
        }
        else{
            auto sibling = path->sibling->recolor(Black);
            auto parent = path->parent->recolor(Red);
            auto sibling_right = sibling->right;
            sibling->right = parent;
            parent->left = sibling_right;
            
            if (path->grandparent != NULL){
                if (path->grandparentComparison == Greater){
                    path->grandparent->right = sibling;
                }else{
                    path->grandparent->left = sibling;
                }
                path->parent = parent;
                path->grandparentComparison = Greater;
                path->grandparent = sibling;
                path->sibling = path->parent->left;
                return doubleBlack(path);
            }else{
                auto newPath = new Path<Key, Value>(path->child);
                newPath = newPath->push(parent,Greater);
                newPath = newPath->push(sibling, Greater);
                return doubleBlack(newPath);
            }
        }
    }
    return path;
}

#endif /* RBTree_hpp */
