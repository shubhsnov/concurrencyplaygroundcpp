#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

mutex myMutex;

class MySingleton {
    public:
    static MySingleton& getInstance() {
        // This to make the singleton thread safe
        lock_guard<mutex> myLock(myMutex);
        if (!instance) instance = new MySingleton();
        return *instance;
    }

    private:
    MySingleton();
    ~MySingleton();
    //No copy operator
    MySingleton(const MySingleton&) = delete;
    MySingleton& operator= (const MySingleton&) = delete;
    static MySingleton* instance;
};


int main() {
    /*
    Problem: Each time I use my singleton in one of the below ways, it will lock the mutex which is slow, especially if I just read from it.

    MySingleton::MySingleton()= default;
    MySingleton::~MySingleton()= default;
    MySingleton* MySingleton::instance= nullptr;
    ...
    MySingleton::getInstance();

    */
    return 0;
}

