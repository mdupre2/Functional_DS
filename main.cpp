//
//  main.cpp
//  FunctionalDS
//
//  Created by Mermer Dupree on 4/20/16.
//  Copyright © 2016 Mermer Dupree. All rights reserved.
//
#include "RBTree.hpp"
#include <stdlib.h>
#include <time.h>
#include "BitVecTri.hpp"
#include "BankersQueue.hpp"
#include <algorithm>
#include <chrono>

#include <map>
#include <stack>
#include <queue>
#include <vector>

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/task_info.h>
#include <mach/mach.h>
#include <mach/message.h>  // for mach_msg_type_number_t
#include <mach/kern_return.h>  // for kern_return_t

using namespace std::chrono;
//#include <iostream>
//#include <string>

//#include "FQueue.hpp"
//#include "List.hpp"
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



int main(int argc, const char * argv[]) {
    
    srand (time(NULL));
    char arv[] = "test_correctness_rbtree";
    long  runtime = 0;
    long long mem = 0;
    string dataStruct = (argc > 1) ? argv[1] : "bitvectri";
    string mode = "efficiancy";
    string access = (argc > 3) ? argv[3] : "back";
    string add_or_delete = (argc > 2) ? argv[2] : "insert";
    int ntests = 1000000;
    int mx = 1000000;
    int grandularity = 10; // how many times you take a stapshot of time and memory during tests
    string title;
    
    unsigned long long* memoryLog = new unsigned long long[grandularity + 1];
    long* timeLog = new  long[grandularity + 1];
    
    //create random insert list
    int* rand_insert_arr = new int[ntests];
    for(int i = 0; i < ntests; i++){
        rand_insert_arr[i] = i;
    }
    random_shuffle(&rand_insert_arr[0], &rand_insert_arr[ntests]);
    
    //create random delete list
    int* rand_delete_arr = new int[ntests];
    for(int i = 0; i < ntests; i++){
        rand_delete_arr[i] = i;
    }
    random_shuffle(&rand_delete_arr[0], &rand_delete_arr[ntests]);
    
    
    if (mode.compare("correctness") == 0){
        if (dataStruct.compare("rbtree") == 0 ){
            RBTree<int, string>* rbt = new RBTree<int, string>();
            // Test inserts
            cout << "Testing Inserts:" << endl;
            for( int i = 0; i < 10000; i++){
                int k = rand() % 1000;
                cout << "Inserting " << k << endl;
                rbt = rbt->insert(k, "test");
                if (rbt->isValid()){
                    cout << "Valid" << endl;
                }else{
                    cout << "Not Valid" << endl;
                    break;
                }
            }
            cout << "\nTesting Deletes:" << endl;
            for( int i = 0; i < 10000; i++){
                int k = rand() % 1000;
                cout << "Removing " << k << endl;
                rbt = rbt->remove(k);
                if (rbt->isValid()){
                    cout << "Valid" << endl;
                }else{
                    cout << "Not Valid" << endl;
                    rbt->image("notvalid");
                    cout << endl << endl;
                    cout << "Test Fail." << endl;
                    return 1;
                }
            }
            cout << endl << endl;
            cout << "Test Passed." << endl;
            
        }
        else if (dataStruct.compare("bitvectri") == 0){
            BitVecTri<int>* bvt = new BitVecTri<int>();
            int n = 1000;
            for( int i = 0; i < n-1; i++){
                bvt = bvt->append(i);
            }
            bvt = bvt->append(n-1);
            bvt = bvt->update(777, 0);
            for( int i = 0; i < n-1; i++){
                cout << i << ": " << bvt->nth(i) << endl;
            }
            cout << bvt->nth(n-1) << endl;
            cout << "Size: " << bvt->length() << endl;
            return 0;
            
        }
        else if (dataStruct.compare("queue") == 0){
            
            
        }
        else if (dataStruct.compare("list") == 0){
            
            
        }
    }
    else if (mode.compare("efficiancy") == 0){
        
        if (dataStruct.compare("rbtree") == 0 ){
            if (access.compare("front") == 0){
                if (add_or_delete.compare("insert") == 0){
                    title = "Inserting Objects Into Red Black Tree In Order";
                    RBTree<int, string>* rbt = new RBTree<int, string>();
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = rbt->memoryUsed();
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        rbt = rbt->insert(i, "test");
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = rbt->memoryUsed();
                    timeLog[grandularity] = runtime;
                }
                else if (add_or_delete.compare("delete") == 0){
                    title = "Deleting Objects In A Red Black Tree In Order";
                    RBTree<int, string>* rbt = new RBTree<int, string>();
                    for (int i = 0; i < ntests; i++){
                        rbt = rbt->insert(i, "test");
                    }
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = rbt->memoryUsed();
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        rbt = rbt->remove(i);
                    }
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = rbt->memoryUsed();
                    timeLog[grandularity] = runtime;
                    
                }
            }
            else if (access.compare("back") == 0){
                if (add_or_delete.compare("insert") == 0){
                    title = "Inserting Objects Into Red Black Tree In Reverse Order";
                    RBTree<int, string>* rbt = new RBTree<int, string>();
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = rbt->memoryUsed();
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        rbt = rbt->insert( ((ntests - i)-1), "test" );
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = rbt->memoryUsed();
                    timeLog[grandularity] = runtime;
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    title = "Deleting Objects In A Red Black Tree In Reverse Order";
                    RBTree<int, string>* rbt = new RBTree<int, string>();
                    
                    for (int i = 0; i < ntests; i++){
                        rbt = rbt->insert(i, "test");
                    }
                    
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = rbt->memoryUsed();
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        rbt = rbt->remove(((ntests - i)-1));
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = rbt->memoryUsed();
                    timeLog[grandularity] = runtime;
                }
            }
            else if (access.compare("random") == 0){
                if (add_or_delete.compare("insert") == 0){
                    title = "Inserting Objects Into Red Black Tree At Random";
                    RBTree<int, string>* rbt = new RBTree<int, string>();
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = rbt->memoryUsed();
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        rbt = rbt->insert(rand_insert_arr[i], "test");
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = rbt->memoryUsed();
                    timeLog[grandularity] = runtime;
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    title = "Deleting Objects In A Red Black Tree At Random";
                    RBTree<int, string>* rbt = new RBTree<int, string>();
                    
                    for (int i = 0; i < ntests; i++){
                       rbt = rbt->insert(rand_insert_arr[i], "test");
                    }
                    
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i > ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = rbt->memoryUsed();
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        rbt = rbt->remove(rand_delete_arr[i]);
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = rbt->memoryUsed();
                    timeLog[grandularity] = runtime;
                }
            }
            
            
        }
        else if (dataStruct.compare("bitvectri") == 0){
            if (access.compare("front") == 0){
                if (add_or_delete.compare("insert") == 0){
                    cout << "Can't insert to front in Bit Vector Tri." << endl;
                    return 0;
                }
                else if (add_or_delete.compare("delete") == 0){
                    title = "Deleting Objects In A Bit Vector Tri From the Front";
                    BitVecTri<string>* bvt = new BitVecTri<string>();
                    
                    for (int i = 0; i < ntests; i++){
                        bvt = bvt->append("test");
                    }
                    
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = bvt->memoryUsed();
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        bvt = bvt->remove(0);
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = bvt->memoryUsed();
                    timeLog[grandularity] = runtime;
                    
                }
            }
            else if (access.compare("back") == 0){
                if (add_or_delete.compare("insert") == 0){
                    title = "Inserting Objects Into A Bit Vector Tri To Front";
                    BitVecTri<string>* bvt = new BitVecTri<string>();
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = bvt->memoryUsed();
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        bvt = bvt->append("test");
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = bvt->memoryUsed();
                    timeLog[grandularity] = runtime;
                }
                else if (add_or_delete.compare("delete") == 0){
                    title = "Deleting Objects In A Bit Vector Tri From the Back";
                    BitVecTri<string>* bvt = new BitVecTri<string>();
                    
                    for (int i = 0; i < ntests; i++){
                        bvt = bvt->append("test");
                    }
                    
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = bvt->memoryUsed();
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        bvt = bvt->removeLast();
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = bvt->memoryUsed();
                    timeLog[grandularity] = runtime;
                }
            }
            else if (access.compare("random") == 0){
                if (add_or_delete.compare("insert") == 0){
                    cout << "Can't insert at random in Bit Vector Tri." << endl;
                    return 0;
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    title = "Deleting Objects In A Bit Vector Tri At Random";
                    BitVecTri<string>* bvt = new BitVecTri<string>();
                    
                    for (int i = 0; i < ntests; i++){
                        bvt = bvt->append("test");
                    }
                    
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = bvt->memoryUsed();
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        bvt = bvt->remove(rand_delete_arr[i]);
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = bvt->memoryUsed();
                    timeLog[grandularity] = runtime;
                    
                }
            }
        }
        else if (dataStruct.compare("bankersqueue") == 0){
            
            if (add_or_delete.compare("insert") == 0){
                title = "Inserting Objects Into A Banker's Queue";
                BankersQueue<string>* bq = new BankersQueue<string>();
                high_resolution_clock::time_point start = high_resolution_clock::now();
                for (int i = 0; i < ntests; i++){
                    if (i % (ntests/grandularity) == 0){
                        memoryLog[i/(ntests/grandularity)] = bq->memoryUsed();
                        high_resolution_clock::time_point end = high_resolution_clock::now();
                        timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                    }
                    bq = bq->enqueue("test");
                }
                
                high_resolution_clock::time_point end = high_resolution_clock::now();
                runtime = duration_cast<microseconds>( end - start ).count();
                memoryLog[grandularity] = bq->memoryUsed();
                timeLog[grandularity] = runtime;
                
            }
            else if (add_or_delete.compare("delete") == 0){
                title = "Deleting Objects From A Banker's Queue";
                BankersQueue<string>* bq = new BankersQueue<string>();
                
                for (int i = 0; i < ntests; i++){
                    bq = bq->enqueue("test");
                }
                
                high_resolution_clock::time_point start = high_resolution_clock::now();
                for (int i = 0; i < ntests; i++){
                    if (i % (ntests/grandularity) == 0){
                        memoryLog[i/(ntests/grandularity)] = bq->memoryUsed();
                        high_resolution_clock::time_point end = high_resolution_clock::now();
                        timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                    }
                    bq = bq->dequeue();
                }
                high_resolution_clock::time_point end = high_resolution_clock::now();
                runtime = duration_cast<microseconds>( end - start ).count();
                memoryLog[grandularity] = bq->memoryUsed();
                timeLog[grandularity] = runtime;
                
            }
                
            
        }
        else if (dataStruct.compare("list") == 0){
           
            if (add_or_delete.compare("insert") == 0){
                title = "Inserting Objects Into A List";
                List<string>* l = new List<string>();
                high_resolution_clock::time_point start = high_resolution_clock::now();
                for (int i = 0; i < ntests; i++){
                    if (i % (ntests/grandularity) == 0){
                        memoryLog[i/(ntests/grandularity)] = l->memoryUsed();
                        high_resolution_clock::time_point end = high_resolution_clock::now();
                        timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                    }
                    l =l->push("test");
                }
                
                high_resolution_clock::time_point end = high_resolution_clock::now();
                runtime = duration_cast<microseconds>( end - start ).count();
                memoryLog[grandularity] = l->memoryUsed();
                timeLog[grandularity] = runtime;
                
            }
            else if (add_or_delete.compare("delete") == 0){
                title = "Deleting Objects From A List";
                List<string>* l = new List<string>();
                
                for (int i = 0; i < ntests; i++){
                    l =l->push("test");
                }
                high_resolution_clock::time_point start = high_resolution_clock::now();
                for (int i = 0; i < ntests; i++){
                    if (i % (ntests/grandularity) == 0){
                        memoryLog[i/(ntests/grandularity)] = l->memoryUsed();
                        high_resolution_clock::time_point end = high_resolution_clock::now();
                        timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                    }
                    l = l->getTail();
                }

                high_resolution_clock::time_point end = high_resolution_clock::now();
                runtime = duration_cast<microseconds>( end - start ).count();
                memoryLog[grandularity] = l->memoryUsed();
                timeLog[grandularity] = runtime;
                
            }
        
        
        }
        // non functional map
        else if (dataStruct.compare("nfmap") == 0){
            if (access.compare("front") == 0){
                if (add_or_delete.compare("insert") == 0){
                    title = "Inserting Objects Into A Non-Functional Map In Order";
                    map<int, string> m = map<int,string>();
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = ( i * (sizeof(int) + sizeof(string("test"))) ) + sizeof(m);
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        m[i] = string("test");
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = ( ntests * (sizeof(int) + sizeof(string("test"))) ) + sizeof(m);
                    timeLog[grandularity] = runtime;
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    title = "Deleting Objects From A Non-Functional Map In Order";
                    map<int, string> m = map<int,string>();
                    
                    for (int i = 0; i < ntests; i++){
                        m[i] = string("test");
                    }
                    int initMem = ( ntests * (sizeof(int) + sizeof(string("test"))) );
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = initMem - ( i * (sizeof(int) + sizeof(string("test"))) ) + sizeof(m);
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        m.erase(i);
                    }
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = sizeof(m);
                    timeLog[grandularity] = runtime;
                    
                }
            }
            else if (access.compare("back") == 0){
                if (add_or_delete.compare("insert") == 0){
                    title = "Inserting Objects Into A Non-Functional Map In Reverse Order";
                    map<int, string> m = map<int,string>();
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = ( i * (sizeof(int) + sizeof(string("test"))) ) + sizeof(m);
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        m[(ntests - 1 ) - i] = string("test");
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = ( ntests * (sizeof(int) + sizeof(string("test"))) ) + sizeof(m);
                    timeLog[grandularity] = runtime;
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    title = "Deleting Objects From A Non-Functional Map In Reverse Order";
                    map<int, string> m = map<int,string>();
                    
                    for (int i = 0; i < ntests; i++){
                        m[i] = string("test");
                    }
                    int initMem = ( ntests * (sizeof(int) + sizeof(string("test"))) );
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = initMem - ( i * (sizeof(int) + sizeof(string("test"))) ) + sizeof(m);
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        m.erase((ntests-i)-1);
                    }
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = sizeof(m);
                    timeLog[grandularity] = runtime;
                    
                }
            }
            else if (access.compare("random") == 0){
                if (add_or_delete.compare("insert") == 0){
                    title = "Inserting Objects Into A Non-Functional Map At Random";
                    map<int, string> m = map<int,string>();
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = ( i * (sizeof(int) + sizeof(string("test"))) ) + sizeof(m);
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        m[rand_insert_arr[i]] = string("test");
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = ( ntests * (sizeof(int) + sizeof(string("test"))) ) + sizeof(m);
                    timeLog[grandularity] = runtime;
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    title = "Deleting Objects From A Non-Functional Map At Random";
                    map<int, string> m = map<int,string>();
                    
                    for (int i = 0; i < ntests; i++){
                        m[rand_insert_arr[i]] = string("test");
                    }
                    int initMem = ( ntests * (sizeof(int) + sizeof(string("test"))) );
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = initMem - ( i * (sizeof(int) + sizeof(string("test"))) ) + sizeof(m);
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        m.erase(rand_delete_arr[i]);
                    }
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = sizeof(m);
                    timeLog[grandularity] = runtime;
                    
                }
            }
            
        }
        //Non-Functional Vector
        else if (dataStruct.compare("nfqueue") == 0){
            if (add_or_delete.compare("insert") == 0){
                title = "Inserting Objects Into A Non-Functional Queue";
                queue<string> q = queue<string>();
                high_resolution_clock::time_point start = high_resolution_clock::now();
                for (int i = 0; i < ntests; i++){
                    if (i % (ntests/grandularity) == 0){
                        memoryLog[i/(ntests/grandularity)] = ( i *  + sizeof(string("test")) )  + sizeof(q);
                        high_resolution_clock::time_point end = high_resolution_clock::now();
                        timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                    }
                    q.push("test");
                }
                
                high_resolution_clock::time_point end = high_resolution_clock::now();
                runtime = duration_cast<microseconds>( end - start ).count();
                memoryLog[grandularity] = ( ntests *  + sizeof(string("test")) ) + sizeof(q);
                timeLog[grandularity] = runtime;
            }
            else if (add_or_delete.compare("delete") == 0){
                title = "Deleting Objects From A Non-Functional Queue";
                queue<string> q = queue<string>();
                for (int i = 0; i < ntests; i++){
                    q.push("test");
                }

                int initMem = ( ntests *  + sizeof(string("test")) );
                
                high_resolution_clock::time_point start = high_resolution_clock::now();
                for (int i = 0; i < ntests; i++){
                    if (i % (ntests/grandularity) == 0){
                        memoryLog[i/(ntests/grandularity)] = ( initMem - ( i *  + sizeof(string("test"))) ) + sizeof(q);
                        high_resolution_clock::time_point end = high_resolution_clock::now();
                        timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                    }
                    q.pop();
                }
                high_resolution_clock::time_point end = high_resolution_clock::now();
                runtime = duration_cast<microseconds>( end - start ).count();
                memoryLog[grandularity] = sizeof(q);
                timeLog[grandularity] = runtime;
                
            }
        
        }
        else if (dataStruct.compare("nfstack") == 0){
                if (add_or_delete.compare("insert") == 0){
                    title = "Inserting Objects Into A Non-Functional Stack";
                    stack<string> s = stack<string>();
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = ( i *  sizeof(string("test")) ) + sizeof(s);
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        s.push("test");
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = ( ntests *  + sizeof(string("test")) ) + sizeof(s);
                    timeLog[grandularity] = runtime;
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    title = "Deleting Objects From A Non-Functional Stack";
                    stack<string> s = stack<string>();
                    for (int i = 0; i < ntests; i++){
                        s.push("test");
                    }
                    
                    int initMem = ( ntests *  + sizeof(string("test")) );
                    
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            memoryLog[i/(ntests/grandularity)] = ( initMem - ( i *  + sizeof(string("test"))) ) + sizeof(s);
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        s.pop();
                    }
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    memoryLog[grandularity] = sizeof(s);
                    timeLog[grandularity] = runtime;
                    
                }
        }
        //Non-Functional Vector
        else if (dataStruct.compare("nfvector") == 0){
            if (add_or_delete.compare("insert") == 0){
                title = "Appending Objects Into A Non-Functional Vector";
                vector<string> v = vector<string>();
                high_resolution_clock::time_point start = high_resolution_clock::now();
                for (int i = 0; i < ntests; i++){
                    if (i % (ntests/grandularity) == 0){
                        memoryLog[i/(ntests/grandularity)] = v.capacity() * sizeof(string);
                        high_resolution_clock::time_point end = high_resolution_clock::now();
                        timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                    }
                    v.push_back("test");
                }
                
                high_resolution_clock::time_point end = high_resolution_clock::now();
                runtime = duration_cast<microseconds>( end - start ).count();
                memoryLog[grandularity] = v.capacity() * sizeof(string);
                timeLog[grandularity] = runtime;
            }
            else if (add_or_delete.compare("delete") == 0){
                title = "Deleting Objects From The Back of A Non-Functional Vector";
                vector<string> v = vector<string>();
                for (int i = 0; i < ntests; i++){
                    v.push_back("test");
                }
                high_resolution_clock::time_point start = high_resolution_clock::now();
                for (int i = 0; i < ntests; i++){
                    if (i % (ntests/grandularity) == 0){
                        memoryLog[i/(ntests/grandularity)] = v.capacity() * sizeof(string);
                        high_resolution_clock::time_point end = high_resolution_clock::now();
                        timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                    }
                    v.pop_back();
                }
                high_resolution_clock::time_point end = high_resolution_clock::now();
                runtime = duration_cast<microseconds>( end - start ).count();
                memoryLog[grandularity] = v.capacity() * sizeof(string);
                timeLog[grandularity] = runtime;
                
            }
            
        }
        
    }
    string timefn = title + " - time.csv";
    string memfn = title + " - mem.csv";
    std::ofstream time_of(timefn);
    std::ofstream mem_of(memfn);
    
    // print mem
    cout << "Memory Cost of " << title << ", " << endl;
    mem_of << "Memory Cost of " << title << ", " << endl;
    cout << "Objects Inserted" << ", " << "Memory Used (KB)" << endl;
    mem_of << "Objects Inserted" << ", " << "Memory Used (KB)" << endl;
    for (int i = 0; i <= grandularity; i++){
        cout << i*(ntests/grandularity) << ", " << memoryLog[i] / 1000 << endl;
        mem_of << i*(ntests/grandularity) << ", " << memoryLog[i] / 1000 << endl;
    }
    
    cout << endl;
    
    // print time
    cout << "Time Cost of " << title << ", " << endl;
    time_of << "Time Cost of " << title << ", " << endl;
    cout << "Objects Inserted" << ", " << "Time Used (ms)" << endl;
    time_of << "Objects Inserted" << ", " << "Time Used (ms)" << endl;
    for (int i = 0; i <= grandularity; i++){
        cout << i*(ntests/grandularity) << ", " << timeLog[i] / 1000 << endl;
        time_of << i*(ntests/grandularity) << ", " << timeLog[i] / 1000 << endl;
    }
    
    cout << endl <<  "Total Time: " << runtime << endl;
   
    cout << "Total Memory: " << memoryLog[grandularity] / 1000  << endl;
    
    cout << endl << endl << endl;
    return 0;
}
