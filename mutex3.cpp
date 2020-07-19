#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

mutex coutMutex;

struct Worker {
    // storing reference
    string name;
    public:
        Worker(string n): name(n) {};
        void operator() () {
            for (unsigned int j = 0; j <= 3; j++) {
                this_thread::sleep_for(chrono::milliseconds(200));
                lock_guard<mutex> coutGuard(coutMutex);
                cout << name << ": " << "Work " << j << " done!!!" << endl;
            }
        }
};


int main() {
    cout << "Let's start working!!" << endl;
    thread a = thread(Worker("A"));
    thread b = thread(Worker("B"));
    thread c = thread(Worker("C"));
    thread d = thread(Worker("D"));
    thread e = thread(Worker("E"));
    thread f = thread(Worker("F"));

    a.join();
    b.join();
    c.join();
    d.join();
    e.join();
    f.join();
    cout << "Let's go home!!" << endl;

    /* We use mutex to lock the cout operation

    Let's start working!!
        A: Work 0 done!!!
        F: Work 0 done!!!
        C: Work 0 done!!!
        E: Work 0 done!!!
        B: Work 0 done!!!
        D: Work 0 done!!!
        A: Work 1 done!!!
        F: Work 1 done!!!
        D: Work 1 done!!!
        C: Work 1 done!!!
        E: Work 1 done!!!
        B: Work 1 done!!!
        F: Work 2 done!!!
        A: Work 2 done!!!
        D: Work 2 done!!!
        B: Work 2 done!!!
        C: Work 2 done!!!
        E: Work 2 done!!!
        F: Work 3 done!!!
        A: Work 3 done!!!
        D: Work 3 done!!!
        B: Work 3 done!!!
        C: Work 3 done!!!
        E: Work 3 done!!!
        Let's go home!!
    */
    return 0;
}

