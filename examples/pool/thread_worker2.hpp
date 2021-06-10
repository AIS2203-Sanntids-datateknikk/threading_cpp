
#ifndef THREADING_CPP_THREAD_WORKER2_HPP
#define THREADING_CPP_THREAD_WORKER2_HPP

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <utility>

namespace example {

    template<typename T>
    class thread_worker2 {

    public:

        thread_worker2() {
            thread_ = std::thread(&thread_worker2::listen, this);
        }

        std::future<T> work(std::function<T()> f) {

            task_ = std::move(f);
            {
                std::lock_guard<std::mutex> lck(mutex_);
                ready_ = true;
            }

            cv_.notify_one();

            p_ = std::promise<T>();
            return p_.get_future();
        }

        ~thread_worker2() {
            stop_ = true;
            {
                std::lock_guard<std::mutex> lk(mutex_);
                ready_ = true;
            }

            cv_.notify_one();
            thread_.join();
        }

    private:
        bool ready_ = false;
        bool stop_ = false;

        std::thread thread_;
        std::function<T()> task_;

        std::mutex mutex_;
        std::condition_variable cv_;

        std::promise<T> p_;

        void listen() {

            while (!stop_) {

                std::unique_lock<std::mutex> lk(mutex_);
                cv_.wait(lk, [this] {
                    return ready_;
                });

                if (stop_) break;

                p_.set_value(task_());

                ready_ = false;

                lk.unlock();
                cv_.notify_all();
            }
        }
    };

}// namespace example

#endif//THREADING_CPP_THREAD_WORKER_HPP
