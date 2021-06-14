
#include <future>
#include <iostream>

int main() {

    auto f1 = std::async([]{
        return 2;
    });

    auto f2 = std::async([]{
      return 2;
    });

    std::cout << "2+2=" << f1.get() + f2.get() << std::endl;

}