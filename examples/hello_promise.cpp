
#include <thread>
#include <future>
#include <iostream>

int main() {

    std::promise<int> promise;
    auto future = promise.get_future();

    std::thread t([&] {

      std::this_thread::sleep_for(std::chrono::milliseconds(1000));

      promise.set_value(42);

      std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    });

    auto result = future.get(); // blocks until promise has a value
    std::cout << "Answer to the universe is " << result << std::endl;

    t.join(); // waits for t to finish

    return 0;

}