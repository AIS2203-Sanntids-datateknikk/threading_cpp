
#ifndef THREADING_CPP_THREAD_POOL_HPP
#define THREADING_CPP_THREAD_POOL_HPP

#include "thread_worker.hpp"

#include <vector>

namespace example {

    class thread_pool {

    public:
        thread_pool(unsigned int num_threads = std::thread::hardware_concurrency()): pool_(num_threads) {
            for (int i = 0; i < num_threads; i++) {
                pool_.emplace_back(thread_worker());
            }
        }

        ~thread_pool() {
            for (auto &t : pool_) {
                t.stop_and_join();
            }
        }

    private:
        std::vector<std::thread> pool_;

    };


}

#endif//THREADING_CPP_THREAD_POOL_HPP
