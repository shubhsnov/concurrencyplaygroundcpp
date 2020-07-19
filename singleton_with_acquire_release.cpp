/*
A different approach to look at it is to look at the problem from the point of view of reordering reads and writes, both atomic and ordinary:

All atomic operations are guaranteed to be atomic within themselves (the combination of two atomic operations is not atomic as a whole!) and to be visible in the total order in which they appear on the timeline of the execution stream. That means no atomic operation can, under any circumstances, be reordered, but other memory operations might very well be. Compilers (and CPUs) routinely do such reordering as an optimization.
It also means the compiler must use whatever instructions are necessary to guarantee that an atomic operation executing at any time will see the results of each and every other atomic operation, possibly on another processor core (but not necessarily other operations), that were executed before.

Now, a relaxed is just that, the bare minimum. It does nothing in addition and provides no other guarantees. It is the cheapest possible operation. For non-read-modify-write operations on strongly ordered processor architectures (e.g. x86/amd64) this boils down to a plain normal, ordinary move.

The sequentially consistent operation is the exact opposite, it enforces strict ordering not only for atomic operations, but also for other memory operations that happen before or after. Neither one can cross the barrier imposed by the atomic operation. Practically, this means lost optimization opportunities, and possibly fence instructions may have to be inserted. This is the most expensive model.

A release operation prevents ordinary loads and stores from being reordered after the atomic operation, whereas an acquire operation prevents ordinary loads and stores from being reordered before the atomic operation. Everything else can still be moved around.
The combination of preventing stores being moved after, and loads being moved before the respective atomic operation makes sure that whatever the acquiring thread gets to see is consistent, with only a small amount of optimization opportunity lost.
One may think of that as something like a non-existent lock that is being released (by the writer) and acquired (by the reader). Except... there is no lock.

In practice, release/acquire usually means the compiler needs not use any particularly expensive special instructions, but it cannot freely reorder loads and stores to its liking, which may miss out some (small) optimization opportuntities.

Finally, consume is the same operation as acquire, only with the exception that the ordering guarantees only apply to dependent data. Dependent data would e.g. be data that is pointed-to by an atomically modified pointer.
Arguably, that may provide for a couple of optimization opportunities that are not present with acquire operations (since fewer data is subject to restrictions), however this happens at the expense of more complex and more error-prone code, and the non-trivial task of getting dependency chains correct.

It is currently discouraged to use consume ordering while the specification is being revised.

CPPMEM - online tool to show all possible interleavings of the threads
*/
#include <atomic>
#include <mutex>

// sequentially consistent is normal, aqcuire-release is commented (no reordering optimization allowed between the load and store)
using namespace std;
class MySingleton {
    static atomic <MySingleton *> instance;
    static mutex myMutex;
    public:
    static MySingleton * getInstance() {
        MySingleton * sin = instance.load(); // instance.load(memory_order_acquire);
        if (!sin) {
            lock_guard<mutex> myLock(myMutex);
            if (!sin) {
                sin = new MySingleton();
                instance.store(sin); // instance.store(sin, memory_order_release)
            }
        }
        return sin;
    }
};