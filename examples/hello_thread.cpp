#include <chrono>
#include <iostream>
#include <thread>


int main() {

    std::thread t([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "Hello, World!" << std::endl;
    });

    std::cout << "Waiting for thread to finish.." << std::endl;
    t.join();

    return 0;
}
