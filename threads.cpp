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
using namespace std;
int main() {
    thread t([] {
        cout << this_thread::get_id() << endl;
    });
    t.join();

    // t.detach(); Potential risk : if the child outlive the main thread then std::cout will go away with the main thread so we'll undefined behaviour
    // Do detach only when the thread has no references in the main thread
    return 0;
}

