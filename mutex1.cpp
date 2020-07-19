/*
Observers
---------
(public member function) - joinable - checks whether the thread is joinable, i.e. potentially running in parallel context

(public member function) - get_id - returns the id of the thread

(public member function) - native_handle - returns the underlying implementation-defined thread handle

Operations
----------
(public member function) - join - waits for a thread to finish its execution

(public member function) - detach - permits the thread to execute independently from the thread handle

(public member function) - swap - swaps two thread objects

*/
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

struct Worker {
    // storing reference
    string name;
    public:
        Worker(string n): name(n) {};
        void operator() () {
            for (unsigned int j = 0; j <= 3; j++) {
                this_thread::sleep_for(chrono::milliseconds(200));
                cout << name << ": " << "Work " << j << " done!!!" << endl;
            }
        }
};


int main() {
    int valSleeper = 1000;
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

    /* We get interleaved output, because even though std:cout is thread safe, the thread safety is for every characted push to the output stream but not the entire operation, so another thread can pause the cout operation of a thread and start using cout for its own purpose

    Let's start working!!
    D: Work E: Work 0 done!!!
    CA: : Work Work 0 done!!!
    0 done!!!
    F: Work 0 done!!!
    0 done!!!
    B: Work 0 done!!!
    E: Work C: Work 11 done!!!
    done!!!D
    : BAF: Work 1: Work 1 done!!!Work
    : Work 1 done!!!
    1 done!!!
    done!!!
    E: B: Work Work 2 done!!!
    2 done!!!D
    FA: Work 2 done!!!
    : : C: Work Work 2 done!!!
    2 done!!!
    Work 2 done!!!
    EF: DWork : Work C3:  done!!!Work 3 done!!!
    AB: : Work 3 done!!!
    Work 3 done!!!
    3
    : Work  done!!!
    3 done!!!
    Let's go home!!


    Solution is to use mutexes
    */
    return 0;
}

