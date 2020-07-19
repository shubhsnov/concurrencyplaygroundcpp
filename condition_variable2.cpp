#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex mutex_;
condition_variable condVar;
bool dataReady = false;
void waitingForWork() {
    unique_lock<mutex> lck(mutex_);
    condVar.wait(lck, [] {
        return dataReady;
    });

    // do something
}

void setDataReady() {
    {
        lock_guard<mutex> lck(mutex_);
        dataReady = true;
    }
    condVar.notify_one();
}


int main() {
    thread t1 = thread(waitingForWork);
    thread t2 = thread(setDataReady);

    t1.join(), t2.join();
    return 0;

    //Problem: spurious wakeups - sometimes the consumer gets a false notification, and it resumes work
    // Solution - provide additional predicate
}

