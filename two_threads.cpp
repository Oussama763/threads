#include <iostream>
#include <thread>
#include <chrono> //just for the sleep function

void workerFunction(int threadID, int delaySeconds) {
    std::cout << "Thread " << threadID << " is starting...\n";
    std::this_thread::sleep_for(std::chrono::seconds(delaySeconds));
    std::cout << "Thread " << threadID << " has finished!\n";
}

int main() {
    std::cout << "Main thread starting.\n";
    std::thread t1(workerFunction, 1, 3); // Takes 3 seconds
    std::thread t2(workerFunction, 2, 1); // Takes 1 second

    std::cout << "Main thread is waiting for workers to finish...\n";

    t1.join();
    t2.join();

    std::cout << "All threads finished. Exiting main.\n";
    return 0;
}
