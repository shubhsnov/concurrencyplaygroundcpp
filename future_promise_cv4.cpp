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
#include <future>
using namespace std;

int getRandomTime(int start, int end);
class Worker
{
    // storing reference
    string name;

public:
    Worker(string n) : name(n){};
    void operator()(promise<void> && preparedWork, shared_future<void> boss2WorkerStartWork, promise<void> && doneWork, shared_future<void> boss2WorkerDoneWork)
    {
        int preparedTime = getRandomTime(5000, 7500);

        // Sleep for a while to simulate work preparation
        this_thread::sleep_for(std::chrono::milliseconds(preparedTime));

        preparedWork.set_value();
        std::cout << name << ": "
                  << "Work prepared after " << preparedTime << " milliseconds. " << endl;

        boss2WorkerStartWork.wait();

        int workTime = getRandomTime(2000, 4000);
        this_thread::sleep_for(std::chrono::milliseconds(workTime));

        doneWork.set_value();
        std::cout << name << ": "
                  << "Work done after " << workTime << " milliseconds. " << endl;

        boss2WorkerDoneWork.wait();
    }
};

int getRandomTime(int start, int end)
{
    srand(time(0));
    return start + (rand() % (start + end));
}

int main()
{
    promise<void> startWorkProm;
    promise<void> goHomeProm;

    shared_future<void> startWorkFut = startWorkProm.get_future();
    shared_future<void> goHomeFut = goHomeProm.get_future();

    promise<void> aPrepared;
    future<void> waitForAPrepared = aPrepared.get_future();
    promise<void> aDone;
    future<void> waitForADone = aDone.get_future();

    promise<void> bPrepared;
    future<void> waitForBPrepared = bPrepared.get_future();
    promise<void> bDone;
    future<void> waitForBDone = bDone.get_future();

    promise<void> cPrepared;
    future<void> waitForCPrepared = cPrepared.get_future();
    promise<void> cDone;
    future<void> waitForCDone = cDone.get_future();

    promise<void> dPrepared;
    future<void> waitForDPrepared = dPrepared.get_future();
    promise<void> dDone;
    future<void> waitForDDone = dDone.get_future();

    promise<void> ePrepared;
    future<void> waitForEPrepared = ePrepared.get_future();
    promise<void> eDone;
    future<void> waitForEDone = eDone.get_future();

    promise<void> fPrepared;
    future<void> waitForFPrepared = fPrepared.get_future();
    promise<void> fDone;
    future<void> waitForFDone = fDone.get_future();

    Worker a("A");
    thread aWork(a, move(aPrepared), startWorkFut, move(aDone), goHomeFut);
    Worker b("B");
    thread bWork(b, move(bPrepared), startWorkFut, move(bDone), goHomeFut);
    Worker c("C");
    thread cWork(c, move(cPrepared), startWorkFut, move(cDone), goHomeFut);
    Worker d("D");
    thread dWork(d, move(dPrepared), startWorkFut, move(dDone), goHomeFut);
    Worker e("E");
    thread eWork(e, move(ePrepared), startWorkFut, move(eDone), goHomeFut);
    Worker f("f");
    thread fWork(f, move(fPrepared), startWorkFut, move(fDone), goHomeFut);

    std::cout << "BOSS: PREPARE YOUR WORK!!" << endl;
    waitForAPrepared.wait();
    waitForBPrepared.wait();
    waitForCPrepared.wait();
    waitForDPrepared.wait();
    waitForEPrepared.wait();
    waitForFPrepared.wait();
    std::cout << "BOSS: START YOUR WORK!!" << endl;
    startWorkProm.set_value();
    waitForADone.wait();
    waitForBDone.wait();
    waitForCDone.wait();
    waitForDDone.wait();
    waitForEDone.wait();
    waitForFDone.wait();
    std::cout << "BOSS: GO HOME!!" << endl;
    goHomeProm.set_value();

    // Always join at the end
    aWork.join(), bWork.join(), cWork.join(), dWork.join(), eWork.join(), fWork.join();
    return 0;
}

/*
 Output : (still needs lock for cout)
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
