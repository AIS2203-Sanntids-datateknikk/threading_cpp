
#ifndef THREADING_CPP_THREAD_POOL_HPP
#define THREADING_CPP_THREAD_POOL_HPP

#include "thread_worker.hpp"

#include <vector>
#include <any>
#include <future>

namespace example {

    class thread_pool {

    public:
        thread_pool(unsigned int num_threads = std::thread::hardware_concurrency()): available_(num_threads), busy_(num_threads) {
            for (unsigned int i = 0; i < num_threads; i++) {
                available_.emplace_back(std::make_unique<thread_worker<std::any>>());
            }
        }

        template<class T>
        std::future<T> submit(std::function<T()> f) {

        }

        ~thread_pool() {
            for (auto &t : available_) {
                //t.stop_and_join();
            }
        }

    private:
        std::mutex mutex_;
        std::condition_variable cv_;
        std::vector<std::unique_ptr<thread_worker<T>> available_;
        std::vector<std::unique_ptr<thread_worker>> busy_;
    };


}

#endif//THREADING_CPP_THREAD_POOL_HPP
