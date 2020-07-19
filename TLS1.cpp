#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

mutex coutMutex;

//Each thread has its own copy of thread local data
thread_local string s("hello from ");

void addThreadLocal(string const& s2) {
    s += s2;
    lock_guard<mutex> lg(coutMutex);
    cout << s << endl;
    cout << "&s: " << &s << endl;
    cout << endl;
}


int main() {
    thread t1 = thread(addThreadLocal, "t1");
    thread t2 = thread(addThreadLocal, "t2");
    thread t3 = thread(addThreadLocal, "t3");
    thread t4 = thread(addThreadLocal, "t4");

    t1.join(), t2.join(), t3.join(), t4.join();
    return 0;
}

