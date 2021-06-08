
#include "semaphore.hpp"
#include <iostream>
#include <thread>
#include <vector>

int main() {

    std::mutex m;
    example::semaphore semaphore(5);

    const int num_threads = 100;
    std::thread threads[num_threads];
    for (auto i = 0; i < num_threads; i++) {
        threads[i] = std::thread([&m, &semaphore, i] {
            semaphore.acquire();

          {
              std::lock_guard<std::mutex> lock(m);
              std::cout << "hello from thread " << i << ", count=" << semaphore.count() << std::endl;
          }

            semaphore.release();
        });
    }

    for (auto &t : threads) {
        t.join();
    }

    return 0;
}