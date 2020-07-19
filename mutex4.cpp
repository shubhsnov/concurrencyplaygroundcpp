#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

struct CriticalData {
    mutex mut;
};

void lock(CriticalData &a, CriticalData &b) {
    lock_guard<mutex> g1(a.mut);
    cout << "get the first mutex" << endl;
    this_thread::sleep_for(chrono::milliseconds(1));
    lock_guard<mutex> g2(b.mut);
    cout << "get the second mutex" << endl;

    // do some thing with a and b
}


int main() {
    CriticalData c1, c2;
    thread a = thread([&] { lock(c1, c2); });
    // c1 and c2 in reverse - change the direction of the lock
    thread b = thread([&] { lock(c2, c1); });

    a.join();
    b.join();

    // Will result in a deadlock
    //
    // get the first mutex
    // get the first mutex
    // --hang
    // a waits for b to unlock from the first thread and b waits for a to unlock from the second thread

    // Solution can be to correct the direction of lock
    // or we can use unique_lock
    return 0;
}

