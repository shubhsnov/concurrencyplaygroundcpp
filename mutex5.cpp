#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

struct CriticalData {
    mutex mut;
};

void deadLock(CriticalData &a, CriticalData &b) {
    // defer_lock waits the actual acquition till the final lock statment so that
    // all the locks are acquired at once and there is no posibility of deadlock
    unique_lock<mutex> g1(a.mut, defer_lock);
    cout << "Thread: " << this_thread::get_id() << " :get the first mutex" << endl;
    this_thread::sleep_for(chrono::milliseconds(1));
    unique_lock<mutex> g2(b.mut, defer_lock);
    cout << "     Thread: " << this_thread::get_id() << " :get the second mutex" << endl;

    cout << "            Thread: " << this_thread::get_id() << " :get both mutex" << endl;

    lock(g1, g2);
}


int main() {
    CriticalData c1, c2;
    thread a = thread([&] { deadLock(c1, c2); });
    // c1 and c2 in reverse - change the direction of the lock
    thread b = thread([&] { deadLock(c2, c1); });

    a.join();
    b.join();

    // Output :
    // Thread: 0x70000748c000 :get the first mutex
    // Thread: 0x700007409000 :get the first mutex
    //      Thread: 0x70000748c000 :get the second mutex
    //             Thread: 0x70000748c000 :get both mutex // This is where the real acquisition happens
    //      Thread: 0x700007409000 :get the second mutex
    //             Thread: 0x700007409000 :get both mutex
    return 0;
}

