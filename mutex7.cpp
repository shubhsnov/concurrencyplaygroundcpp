#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

mutex myMutex;

class MySingleton {
    public:
    static MySingleton& getInstance() {
        // This to make the singleton thread safe
        lock_guard<mutex> myLock(myMutex);
        if (!instance) instance = new MySingleton();
        return *instance;
    }

    private:
    MySingleton();
    ~MySingleton();
    //No copy operator
    MySingleton(const MySingleton&) = delete;
    MySingleton& operator= (const MySingleton&) = delete;
    static MySingleton* instance;
};


int main() {
    /*
    Problem: Each time I use my singleton in one of the below ways, it will lock the mutex which is slow, especially if I just read from it.

    MySingleton::MySingleton()= default;
    MySingleton::~MySingleton()= default;
    MySingleton* MySingleton::instance= nullptr;
    ...
    MySingleton::getInstance();

    */


    //
    //    Additionally the
    //     new MySingleton();
    //         ------------------- is NOT atomic;

    //         Operations :
    //             1) Allocate memory for MySingleton
    //             2) Create MySingleton object in memory
    //             3) Refer instance to MySingleton object

    //         Possible execution order 1, 3, 2
    return 0;
}

// Solutions
// 1) call_once and once_flags
        // class MySingleton1 {
        //     public:
        //     static MySingleton1& getInstance() {
                   // Will be invoked only once
        //         call_once(initInstanceFlag, &MySingleton1::initSingleton);
        //         return *instance;
        //     }

        //     private:
        //     MySingleton1();
        //     ~MySingleton1();
        //     MySingleton1(const MySingleton1&) = delete;
        //     MySingleton1& operator= (const MySingleton1&) = delete;
               // Important
        //     static once_flag initInstanceFlag;
        //     static void initSingleton() {
        //         instance = new MySingleton1;
        //     }
        //     static MySingleton1* instance;
        // };
//...
        // once_flag MySingleton1::initInstanceFlag;

// 2) Meyers MySingleton

// class MySingleton {
//     public:
//     static MySingleton& getInstance() {
           // static variable are thread safe by nature
//         static MySingleton instance;
//         return *instance;
//     }
//     ...

// 3) Atomic Variables