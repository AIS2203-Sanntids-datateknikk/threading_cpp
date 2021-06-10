
#ifndef THREADING_CPP_THREAD_WORKER_HPP
#define THREADING_CPP_THREAD_WORKER_HPP

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <utility>

namespace example {

    class thread_worker {

    public:

        thread_worker() {
            thread_ = std::thread(&thread_worker::listen, this);
        }

        void work(std::function<void()> f) {
            task_ = std::move(f);
            {
                std::lock_guard<std::mutex> lck(mutex_);
                ready_ = true;
                processed_ = false;
            }

            cv_.notify_one();

        }

        void join(bool stop = false) {
            {
                std::unique_lock<std::mutex> lck(mutex_);
                cv_.wait(lck, [this] {
                    return processed_;
                });
            }

            if (stop) {
                stop_ = true;
                {
                    std::lock_guard<std::mutex> lk(mutex_);
                    ready_ = true;
                }

                cv_.notify_one();
                thread_.join();
            }
        }

        ~thread_worker() {
            if (!stop_) {
                join(true);
            }
        }

    private:
        bool ready_;
        bool processed_;
        bool stop_;

        std::thread thread_;
        std::function<void()> task_;

        std::mutex mutex_;
        std::condition_variable cv_;

        void listen() {
            while (!stop_) {

                std::unique_lock<std::mutex> lk(mutex_);
                cv_.wait(lk, [this] {
                    return ready_;
                });

                if (stop_) break;

                task_();

                ready_ = false;
                processed_ = true;

                lk.unlock();
                cv_.notify_all();
            }
        }
    };

}// namespace example

#endif//THREADING_CPP_THREAD_WORKER_HPP
