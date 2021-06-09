
#ifndef THREADING_CPP_THREAD_WORKER_HPP
#define THREADING_CPP_THREAD_WORKER_HPP

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

namespace example {

    class thread_worker {

    public:
        thread_worker() {
            thread_ = std::thread(&thread_worker::listen, this);
        }

        void work(std::function<void()> f) {
            task_ = f;
            {
                std::lock_guard<std::mutex> lck(mutex_);
                ready_ = true;
                processed_ = false;
            }

            cv_.notify_one();

            td::unique_lock<std::mutex> lk(m_);
            cv_.wait(lk, [this] {
                return processed_;
            });
        }

        void stop_and_join() {
            if (!stop_) {
                stop_ = true;
                {
                    std::lock_guard<std::mutex> lk(mutex_);
                    ready_ = true;
                }
                cv_.notify_one();
                worker_.join();
            }
        }

        ~thread_worker()
        {
            stop_and_join();
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
            while (!stop) {

                std::unique_lock<std::mutex> lk(mutex_);
                cv_.wait(lk, [this] {
                    return ready_;
                });

                if (stop) break;

                task_();

                ready_ = false;
                processed_ = true;

                lk.unlock();
                cv_.notify_one();
            }
        }
    };

}// namespace example

#endif//THREADING_CPP_THREAD_WORKER_HPP
