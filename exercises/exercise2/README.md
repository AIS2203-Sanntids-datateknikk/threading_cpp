#### Exercise 2

A system of three classes: `Producer` and `Consumer`, and a class called `StorageBox` 
as a shared resource for the `Producer` and `Consumer` has been designed.

Finish implementing the methods contained by the `StorageBox` so that it becomes "thread-safe", 
e.g. parts of the class must be synchronized. 
The purpose of this class is to hold an `std::string` value. 
A put()-method to update the value, and a get()-method to read the value. 
A value can not be overwritten before it is read, and it should only be possible to read the value one time.
