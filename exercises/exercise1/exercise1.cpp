
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace {

    struct writer {

        void write(const std::string &str) {
            ss_ += str;
        }

        std::string str() const {
            return ss_;
        }

    private:
        std::string ss_;
    };

}// namespace

int main() {

    std::mutex m;
    std::condition_variable cv;

    bool stop = false;
    bool ready = false;
    writer w;

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        auto t = std::thread([&, i] {
            {
                std::unique_lock<std::mutex> lck(m);
                while (!ready) cv.wait(lck);
            }

            while (!stop) {
                w.write("Hello from thread " + std::to_string(i) + "\n");
            }
        });
        threads.emplace_back(std::move(t));
    }

    {
        std::unique_lock<std::mutex> lck(m);
        ready = true;
        cv.notify_all();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    stop = true;
    std::cout << w.str() << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << w.str() << std::endl;


    for (auto &t : threads) {
        if (t.joinable()) t.join();
    }


    return 0;
}