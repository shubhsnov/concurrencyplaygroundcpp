// Memory Model - Sequential Consistency, the ordering of execution in the thread will be consistent and same. Universal order of instructions.

//Using atomic variables with acquire release semantics
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
using namespace std;

int x; // Only atomic operations allowed
atomic<int> y;

void reading() {
    cout << "x: " << x << endl; //  x is not atomic, it can be read from while it is being written
    cout << "y: " << y.load(memory_order_relaxed) << endl;
}

void writing() {
    y.store(2000, memory_order_relaxed);
    x = 11;
}


int main() {
    y.store(0);

    // For atomic variables the order of threads is what the order they are executed in
    thread writer = thread(writing);
    thread reader = thread(reading);

    writer.join();
    reader.join();
    return 0;
}
