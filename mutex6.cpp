#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

mutex coutMutex;

void lock() {
    cout << "Still waiting ..." << endl;
    lock_guard<mutex> lg(coutMutex);
    cout << this_thread::get_id() << endl;
}


int main() {
    thread a = thread([&] { lock(); });

    //deadblock situation
    // {
    //     lock_guard<mutex> lg(coutMutex);
    //     cout << this_thread::get_id() << endl;
        //the above mutex is shared by the main thread and until this scope exits the
        //the mutex will not unlock, but the scope will exit only when thread completes execution, but the thread cannot complete execution since cout is guarded
    //     a.join();
    // }

    //Solution is to change position of join
    {
        a.join();
        lock_guard<mutex> lg(coutMutex);
        cout << this_thread::get_id() << endl;
        //the above mutex is shared by the main thread and until this scope exits the
        //the mutex will not unlock, but the scope will exit only when thread completes execution, but the thread cannot complete execution since cout is guarded
    }
    return 0;
}

