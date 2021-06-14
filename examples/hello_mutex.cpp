
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

int main() {

    std::mutex mutex;
    mutex.lock();

    std::thread t([&]{
        std::cout << "Thread waiting..";
        mutex.lock();
        mutex.unlock();
        std::cout << " done.." << std::endl;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    mutex.unlock();

    t.join();

    return 0;

}