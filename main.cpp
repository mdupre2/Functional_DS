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



// code borrowed from http://stackoverflow.com/questions/18389581/memory-used-by-a-process-under-mac-os-x
unsigned long long memoryUsed(){
    kern_return_t error;
    mach_msg_type_number_t outCount;
    mach_task_basic_info_data_t taskinfo;
    
    taskinfo.virtual_size = 0;
    outCount = MACH_TASK_BASIC_INFO_COUNT;
    error = task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&taskinfo, &outCount);
    if (error == KERN_SUCCESS) {
        // type is mach_vm_size_t
        //printf("vsize = %llu\n", (unsigned long long)taskinfo.virtual_size);
        return (unsigned long long)taskinfo.virtual_size;//resident_size;//
    } else {
        printf("error %d\n", (int)error);
        return -1;
    }
}


int main(int argc, const char * argv[]) {
    //cout << memoryUsed() << endl;
    srand (time(NULL));
    char arv[] = "test_correctness_rbtree";
    long  runtime = 0;
    long long mem = 0;
    string dataStruct = (argc > 1) ? argv[1] : "bankersqueue";
    string mode = "efficiancy";
    string access = (argc > 3) ? argv[3] : "random";
    string add_or_delete = (argc > 2) ? argv[2] : "add";
    //cout << dataStruct << endl << add_or_delete << endl << access << endl;
    int ntests = 1000000;
    int mx = 1000000;
    int grandularity = 10;
    string title;
    //bool outputMem = add_or_delete.compare("insert");
    //time and memory log
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
                    title = "Inserting Objects Into Red Black Tree From Front";
                    //cout << title << endl;
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
                    title = "Deleting Objects In A Red Black Tree From Front";
                    RBTree<int, string>* rbt = new RBTree<int, string>();
                    
                    for (int i = 0; i < ntests; i++){
                        rbt = rbt->insert(i, "test");
                    }
                    
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = 0; i < ntests; i++){
                        if (i % (ntests/grandularity) == 0){
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        rbt = rbt->remove(i);
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
                    timeLog[grandularity] = runtime;
                    
                }
            }
            else if (access.compare("back") == 0){
                if (add_or_delete.compare("insert") == 0){
                    title = "Inserting Objects Into Red Black Tree From Front";
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
                    title = "Deleting Objects In A Red Black Tree From Back";
                    RBTree<int, string>* rbt = new RBTree<int, string>();
                    
                    for (int i = 0; i < ntests; i++){
                        rbt = rbt->insert(i, "test");
                    }
                    
                    high_resolution_clock::time_point start = high_resolution_clock::now();
                    for (int i = ntests -1; i >= 0; i--){
                        if (i % (ntests/grandularity) == 0){
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        rbt = rbt->remove(i);
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
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
                            high_resolution_clock::time_point end = high_resolution_clock::now();
                            timeLog[i/(ntests/grandularity)] = duration_cast<microseconds>( end - start ).count();;
                        }
                        rbt = rbt->remove(rand_delete_arr[i]);
                    }
                    
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    runtime = duration_cast<microseconds>( end - start ).count();
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
            else if (access.compare("Back") == 0){
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
                        bvt = bvt->remove(rand() % (ntests-i));
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
        else if (dataStruct.compare("map") == 0){
            if (access.compare("front") == 0){
                if (add_or_delete.compare("insert") == 0){
                    
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    
                    
                }
            }
            else if (access.compare("last") == 0){
                if (add_or_delete.compare("insert") == 0){
                    
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    
                    
                }
            }
            else if (access.compare("random") == 0){
                if (add_or_delete.compare("insert") == 0){
                    
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    
                    
                }
            }
            
        }
        else if (dataStruct.compare("nonfunctional_queue") == 0){
            if (access.compare("front") == 0){
                if (add_or_delete.compare("insert") == 0){
                    
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    
                    
                }
            }
            else if (access.compare("back") == 0){
                if (add_or_delete.compare("insert") == 0){
                    
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    
                    
                }
            }
            else if (access.compare("random") == 0){
                if (add_or_delete.compare("insert") == 0){
                    
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    
                    
                }
            }
            
        }
        else if (dataStruct.compare("nontfunctional_stack") == 0){
            if (access.compare("front") == 0){
                if (add_or_delete.compare("insert") == 0){
                    
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    
                    
                }
            }
            else if (access.compare("back") == 0){
                if (add_or_delete.compare("insert") == 0){
                    
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    
                    
                }
            }
            else if (access.compare("random") == 0){
                if (add_or_delete.compare("insert") == 0){
                    
                    
                }
                else if (add_or_delete.compare("delete") == 0){
                    
                    
                }
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
   
    cout << "Total Memory: " << memoryLog[grandularity] << endl;
    
    cout << endl << endl << endl;
    return 0;
    /*
    
    BitVecTri<int>* bvt = new BitVecTri<int>();
    int n = 1;
    for( int i = 0; i < n-1; i++){
        bvt = bvt->append(i);
    }
    bvt = bvt->append(n-1);
    //return 0;
    bvt = bvt->update(777, 0);
    for( int i = 0; i < n-1; i++){
        cout << i << ": " << bvt->nth(i) << endl;
    }
    cout << bvt->nth(n-1) << endl;
    cout << "Size: " << bvt->length() << endl;
    return 0;
    */
    //srand (time(NULL));
    RBTree<int, string>* rbt = new RBTree<int, string>();
    rbt = rbt->insert(52, "two");
    rbt = rbt->insert(0, "zero");
    //rbt->bfprint(3);
    rbt = rbt->insert(0, "one");
    //rbt->bfprint(3);
    //return 0;
    for( int i = 0; i < 10; i++){
        int k = rand() % 100;
        cout << "Inserting " << k << endl;
        rbt = rbt->insert(k, "test");
        if (rbt->isValid()){
            cout << "Valid" << endl;
        }else{
            break;
        }
    }
    //rbt->bfprint(4);
    //return 0;
    
    //rbt = rbt->insert(4, "four");
    //rbt = rbt->insert(0, "zero");
    //rbt = rbt->insert(1, "one");
    
    rbt->bfprint(4);
    rbt  = rbt->remove(52);
    std::cout << "\n\n\n delete 22: \n\n\n";
    rbt->bfprint(4);
    return 0;

    
    rbt = rbt->insert(5, "five");
    rbt = rbt->insert(6, "six");
    
    
    
    rbt = rbt->insert(6, "six");
    rbt->bfprint(4);
    return 0;
    std::cout << "parent: \n";
    rbt->path->parent->print();
    rbt = rbt->insert(4, "four");
    //rbt = rbt->insert(5, "five");
    //rbt = rbt->insert(6, "six");
    std::cout << "uncle: \n";
    //rbt->path->uncle->print();
    rbt->bfprint(3);
    return 0;
}

/*
class Adj {
    Adj* next;
    int val;
};
class  Node{
    Adj* next;
    
};
class Graph{
    int nNodes;
    int *adj;
    
};

void print( List<int> *l){
    if (l->isNull()){
        std::cout << "\n";
        return;
    }
    std::cout << l->getHead() << " " ;
    print(l->getTail());
}

void print( FQueue<int>* q){
    if (q->isEmpty()){
        std::cout << "\n";
        return;
    }else{
        print(q->getFront());
        print(q->getBack());
    }
    
}

int main(int argc, const char * argv[]) {
    
    
    RBTree<string, int>* rbt = new RBTree<string, int>();
    rbt = rbt->insert("one", 1);
    rbt->print();
    
    
    return 0;
    // insert code here...
    List<int>* l1 = new List<int>();
    List<int>* l2 = l1->push(1);
    List<int>* l3 = l2->push(2);
    List<int>* l4 = l3->push(3);
    print(l4);
    List<int>* l5 = (l4->concat(l4->reverse()));
    print(l5);
 //   return 0;
    FQueue<int>* q1 = new FQueue<int>();
    FQueue<int>* q2 = q1->enqueue(1);
    FQueue<int>* q3 = q2->enqueue(2);
    FQueue<int>* q4 = q3->enqueue(3);
    print(q4);
    std::cout << "Hello, World!\n";
    print(q4->dequeue());
    std::cout << "Hello, World!\n";
    //std::cout << l4.len() << "\n";
    return 0;
}
*/
