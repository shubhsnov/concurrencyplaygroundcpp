// Memory Model - Sequential Consistency, the ordering of execution in the thread will be consistent and same. Universal order of instructions.

//Using atomic variables
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
using namespace std;

atomic<int> x;
atomic<int> y;

void reading() {
    cout << "x: " << x.load() << endl;
    cout << "y: " << y.load() << endl;
}

void writing() {
    y.store(2000);
    x.store(11);
}


int main() {
    x.store(0);
    y.store(0);

    // For atomic variables the order of threads is what the order they are executed in
    thread writer = thread(writing);
    thread reader = thread(reading);

    writer.join();
    reader.join();
    return 0;
}

// The outcome will be consistent with sequential consistency. interleaving of threads possible