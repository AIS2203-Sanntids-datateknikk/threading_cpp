#### Exercise 1a

Spawn 10 [thread](https://www.cplusplus.com/reference/thread/thread/) objects that prints some unique number to the console using `std::cout`. 
E.g. use a for loop and print the current index.

Wait for all threads to finish execution (by joining the threads) and terminate. Observe the output.

_Hint_: Store the thread instances in a `std::vector` so that the program does not terminate prematurely. 
However, you may find that threads objects cannot be copied, so you'll need to use `std::move` to move the instances into the vector.
```cpp
std::vector threads;
...
std::thread t(...);
threads.emplace_back(std::move(t));
```

#### Exercise 1b

Output not as expected? 
Try fixing it by introducing a [mutex](https://en.cppreference.com/w/cpp/thread/mutex) that controls access to the `std::cout` shared resource. 
