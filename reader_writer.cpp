#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

int x = 0;
int y = 0;

void reading() {
    cout << "x: " << x << endl;
    cout << "y: " << y << endl;
}

void writing() {
    x = 2000;
    y = 11;
}


int main() {
    thread writer = thread(writing);
    thread reader = thread(reading);

    writer.join();
    reader.join();
    return 0;
}

// The outcome can be anything since the outcome will depend on order of events