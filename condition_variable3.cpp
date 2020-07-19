#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex mutex_;
condition_variable condVar;

void waitingForWork() {
    unique_lock<mutex> lck(mutex_);
    condVar.wait(lck);

    // do something
}

void setDataReady() {
    condVar.notify_one();
}


int main() {
    thread t2 = thread(setDataReady);
    // short time delay
    // this thread becomes alive later than when setDataReady sends notification
    // and thus ends up missing the notification
    thread t1 = thread(waitingForWork);

    t1.join(), t2.join();
    return 0;

    //Problem: Lost wakeup - The condition_variable class is a synchronization primitive that can be used to block a thread or multiple threads "at the same time"
}

