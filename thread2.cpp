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

    thread t2([] {
        cout << this_thread::get_id() << endl;
    });
    //Case 1)
    //t =  std::move(t2);

    // Doing the below after the std::move will result in an exception
    // since move will call the destructor of t, and t will now have the
    // value of t2, and then t2 will ve destroyed.
    // t.join();
    // t2.join();

    //Solution
    t.join();
    cout << "t.joinable" << " " << t.joinable() << endl;//t not joinable after join
    cout << "t2.joinable" << " " << t2.joinable() << endl;//t2 joinable before move
    t =  std::move(t2);
    cout << "t2.joinable" << " " << t2.joinable() << endl;//t2 joinable before move
    cout << "t.joinable" << " " << t.joinable() << endl;//t joinable after move
    t.join(); // will have the lambda of the t2

    cout << "t2.joinable" << " " << t2.joinable() << endl; // t2 will not be joinable after the move

    // scoped_thread (takes thread as argumend in constructor) - we have to explicitely join since not joining can force destruct a lot of the execution going on in the parallel threads in case the main thread exits
    // so the scoped_thread object will be created and when main exits, the ~scoped_thread will be called, which will join the underlying thread, so that we don't have to do it explicitely
    return 0;
}

