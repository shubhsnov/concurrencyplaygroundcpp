/*
Problem:
Boss Thread
    Starts threads
        The threads have to notify the Boss the preparation for work has been done.
    The Boss on getting all the notification with notify all the threads to start work
        The threads will complete work and notify back and exit
    Once all threads complete work Boss goes home
*/
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <cstdlib>
#include <ctime>
using namespace std;

std::atomic<int> preparedCount;
std::atomic<int> doneCount;
condition_variable workerToBossCondVariable;
condition_variable bossToWorkerCondVariable;
bool startWork = false;
bool goHome = false;
mutex startWorkMutex, goHomeMutex, coutMutex;
int getRandomTime(int start, int end);
class Worker {
    // storing reference
    string name;
    public:
        Worker(string n): name(n) {};
        void operator() () {
            int preparedTime = getRandomTime(5000, 7500);

            // Sleep for a while to simulate work preparation
            this_thread::sleep_for(std::chrono::milliseconds(preparedTime));

            // Increment the preparedCount once work is done. This is being tracked by the BOSS
            preparedCount++;

            // Show preparation notice
            coutMutex.lock();
            cout << name <<  ": " << "Work prepared after " << preparedTime << " milliseconds. " << endl;
            coutMutex.unlock();

            // Notify BOSS that work is prepared
            workerToBossCondVariable.notify_one();

            // The below lock will wait for the BOSS notification to start work,
            // and on notifiction only continue if startWork = true
            {
                // wait for the boss
                unique_lock<mutex> startWorkLock(startWorkMutex);
                bossToWorkerCondVariable.wait(startWorkLock, [] {
                    return startWork;
                });
            }

            // Continue to finish the work
            int workTime = getRandomTime(2000, 4000);
            this_thread::sleep_for(std::chrono::milliseconds(workTime));

            // Increment done count, also being tracked by the BOSS
            doneCount++;

            // Show work done notice
            coutMutex.lock();
            cout << name <<  ": " << "Work done after " << workTime << " milliseconds. " << endl;
            coutMutex.unlock();

            // Notify boss of completion
            workerToBossCondVariable.notify_one();

            // Wait for Boss's mesage to goHome
            {
                // wait for the boss
                unique_lock<mutex> goHomeLock(goHomeMutex);
                bossToWorkerCondVariable.wait(goHomeLock, [] {
                    return goHome;
                });
            }
        }
};

int getRandomTime(int start, int end) {
    srand(time(0));
    return start + (rand() % (start + end));
}

int main() {
    mutex preparedMutex, doneMutex;
    //Prepared count is an atomic variable
    //Only once instance across all threads
    preparedCount.store(0);
    cout << "Let's start working!!" << endl;

    //Start the threads
    thread a = thread(Worker("A"));
    thread b = thread(Worker("B"));
    thread c = thread(Worker("C"));
    thread d = thread(Worker("D"));
    thread e = thread(Worker("E"));
    thread f = thread(Worker("F"));

    //Start the work message from BOSS
    coutMutex.lock();
    cout << "BOSS: PREPARE YOUR WORK!!" << endl;
    coutMutex.unlock();

    // The Boss waits using a lock
    unique_lock<mutex> prepareUniqueLock(preparedMutex);
    // prepareUniqueLock will be locked. workerToBossCondVariable gets notified
    // one by one by the workers but it only unlocks prepareUniqueLock when preparedCount becomes 6
    workerToBossCondVariable.wait(prepareUniqueLock, [] {
        return preparedCount == 6;
    });

    // Move on to working phase: Setting startWork to will unlock all the threads
    // waiting for the start work notifications. If we don't set this then the threads
    // will keep on waiting even if we notify since it is a predicate check inside
    // the workers.
    startWork = true;
    coutMutex.lock();
    cout << "BOSS: START YOUR WORK!!" << endl;
    coutMutex.unlock();

    // Initialize the done count atomic variable
    doneCount.store(0);

    // Notify all workers to start work
    bossToWorkerCondVariable.notify_all();

    // Same are working phase lock, wait for all to complete work
    unique_lock<mutex> doneUniqueLock(doneMutex);
    workerToBossCondVariable.wait(doneUniqueLock, [] {
        return doneCount == 6;
    });

    // Unblock threads for completion
    goHome = true;
    coutMutex.lock();
    cout << "BOSS: GO HOME!!" << endl;
    coutMutex.unlock();

    // Notify all workers to go home
    bossToWorkerCondVariable.notify_all();

    // Always join at the end
    a.join(), b.join(), c.join(), d.join(), e.join(), f.join();
    return 0;
}

/*
 Output :
 Let's start working!!
BOSS: PREPARE YOUR WORK!!
E: Work prepared after 5709 milliseconds.
D: Work prepared after 6599 milliseconds.
B: Work prepared after 6599 milliseconds.
C: Work prepared after 6599 milliseconds.
F: Work prepared after 6599 milliseconds.
A: Work prepared after 10135 milliseconds.
BOSS: START YOUR WORK!!
C: Work done after 6169 milliseconds.
B: Work done after 6169 milliseconds.
A: Work done after 6169 milliseconds.
F: Work done after 6169 milliseconds.
D: Work done after 6169 milliseconds.
E: Work done after 6169 milliseconds.
BOSS: GO HOME!!

*/