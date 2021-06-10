
#include "thread_worker2.hpp"
#include <iostream>

int main() {

//    example::thread_pool pool;
//
//    for (int i = 0; i < 10; i++) {
//        pool
//    }

    example::thread_worker2<int> worker;
    auto future1 = worker.work([]{
        return 1;
    });

    std::cout << "nils" << std::endl;
    std::cout << future1.get() << std::endl;

    auto future2 = worker.work([]{
      return 2;
    });
    std::cout << future2.get() << std::endl;

//    std::cout << result.get() << std::endl;

//    std::cout << "ole" << std::endl;
//
//    worker.work([]{
//      return "per2";
//    });


}