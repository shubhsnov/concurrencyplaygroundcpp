#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

int x = 0;
int y = 0;
mutex mut;
void reading() {
    lock_guard<mutex> lg(mut);
    cout << "x: " << x << endl;
    cout << "y: " << y << endl;
}

void writing() {
    lock_guard<mutex> lg(mut);
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

// The outcome will depend on which thread gets access to the resource first