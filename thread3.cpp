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

struct Sleeper {
    // storing reference
    int &i;
    public:
        Sleeper(int & i_): i(i_) {};
        void operator() (int k) {
            for (unsigned int j = 0; j <= 5; j++) {
                this_thread::sleep_for(chrono::milliseconds(100));
                i += k;
            }
        }
};


int main() {
    int valSleeper = 1000;
    cout << "valSleeper" << " " << valSleeper << endl;
    thread t(Sleeper(valSleeper), 5);
    // t.detach();
    // cout << "valSleeper" << " " << valSleeper << endl; // will return 1000 since main doesn't wait

    t.join();
    cout << "valSleeper" << " " << valSleeper << endl; // will return 1030 since as main waits

    // NOTE: detaching a thread and doing something by reference is a bad idea
    return 0;
}

