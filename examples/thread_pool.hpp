
#ifndef THREADING_CPP_THREAD_POOL_HPP
#define THREADING_CPP_THREAD_POOL_HPP

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace example {

    class thread_pool {
        std::atomic_bool done;
        std::queue<std::function<void()>> work_queue;
        std::vector<std::thread> threads;
        std::mutex mutex, mutex2;
        std::condition_variable cv;

        void worker_thread() {
            while (!done) {

                std::unique_lock<std::mutex> lock(mutex);
                if (!work_queue.empty()) {

                    auto task = work_queue.front();
                    work_queue.pop();
                    lock.unlock();

                    task();

                    std::lock_guard<std::mutex> lck(mutex2);
                    cv.notify_one();

                } else {
                    std::this_thread::yield();
                }
            }
        }

    public:
        thread_pool() : done(false) {

            unsigned const thread_count = std::thread::hardware_concurrency();
            for (unsigned i = 0; i < thread_count; ++i) {
                threads.emplace_back(&thread_pool::worker_thread, this);
            }
        }

        void wait_for_tasks_to_finish() {


            std::unique_lock<std::mutex> lck(mutex2);
            while (!work_queue.empty()) cv.wait(lck);
            lck.unlock();

            done = true;

            for (auto &thread : threads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
        }

        ~thread_pool() {
            done = true;
        }

        template<typename FunctionType>
        void submit(FunctionType f) {
            work_queue.push(std::function<void()>(f));
        }
    };

}// namespace example

#endif//THREADING_CPP_THREAD_POOL_HPP
