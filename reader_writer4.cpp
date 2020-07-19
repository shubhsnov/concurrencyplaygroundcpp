// Memory Model - Sequential Consistency, the ordering of execution in the thread will be consistent and same. Universal order of instructions.

//Using atomic variables with acquire release semantics
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
using namespace std;

atomic<int> x; // Only atomic operations allowed
atomic<int> y;

void reading() {
    cout << "x: " << x.load(memory_order_acquire) << endl;
    cout << "y: " << y.load(memory_order_relaxed) << endl;
}

void writing() {
    y.store(2000, memory_order_relaxed);
    x.store(11, memory_order_release);
}

// if all relaxed that all interleaving are possible, but with acquire release it will synchronize the between load and store

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


// memory_order_relaxed - Relaxed operation: there are no synchronization or ordering constraints imposed on other reads or writes, only this operation's atomicity is guaranteed (see Relaxed ordering below)

// memory_order_consume - A load operation with this memory order performs a consume operation on the affected memory location: no reads or writes in the current thread dependent on the value currently loaded can be reordered before this load. Writes to data-dependent variables in other threads that release the same atomic variable are visible in the current thread. On most platforms, this affects compiler optimizations only (see Release-Consume ordering below)

// memory_order_acquire - A load operation with this memory order performs the acquire operation on the affected memory location: no reads or writes in the current thread can be reordered before this load. All writes in other threads that release the same atomic variable are visible in the current thread (see Release-Acquire ordering below)

// memory_order_release - A store operation with this memory order performs the release operation: no reads or writes in the current thread can be reordered after this store. All writes in the current thread are visible in other threads that acquire the same atomic variable (see Release-Acquire ordering below) and writes that carry a dependency into the atomic variable become visible in other threads that consume the same atomic (see Release-Consume ordering below).

// memory_order_acq_rel - A read-modify-write operation with this memory order is both an acquire operation and a release operation. No memory reads or writes in the current thread can be reordered before or after this store. All writes in other threads that release the same atomic variable are visible before the modification and the modification is visible in other threads that acquire the same atomic variable.

// memory_order_seq_cst - A load operation with this memory order performs an acquire operation, a store performs a release operation, and read-modify-write performs both an acquire operation and a release operation, plus a single total order exists in which all threads observe all modifications in the same order (see Sequentially-consistent ordering below)