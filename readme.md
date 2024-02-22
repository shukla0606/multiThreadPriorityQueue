MultiThreadedPriorityQueue
This program implements a multi-threaded priority message queue system in C++. Threads can send messages to each other with varying priorities, and upon receiving a message, a simple action is performed concurrently using a thread pool.

Prerequisites
C++ compiler with C++11 support (e.g., g++, clang++)
Standard C++ library
POSIX Threads library (pthread)
Building
Assuming the code is saved in a file named MultiThreadedPriorityQueue.cpp, compile it using the following command:


g++ MultiThreadedPriorityQueue.cpp -o MultiThreadedPriorityQueue -lpthread
This command uses g++ as the compiler, names the executable MultiThreadedPriorityQueue, and links with the pthread library for thread support.

Running
Execute the compiled program using the following command:


./MultiThreadedPriorityQueue
Usage
Overview:

This program demonstrates a multi-threaded priority message queue system.
It includes a priority message queue, a thread pool, and a message sender class.
Threads can send messages to each other with varying priorities.
Upon receiving a message, a simple action is performed concurrently using the thread pool.
MessageSender Configuration:

The MessageSender class is instantiated with a specified number of threads in the pool (e.g., 3 in the provided example).
Sending Messages:

Use the sendMessage method of MessageSender to send messages with specified priorities.
Example:
cpp

messageSender.sendMessage("High Priority Message", 1);
messageSender.sendMessage("Medium Priority Message", 2);
messageSender.sendMessage("Low Priority Message", 3);
Testing:

The provided main function demonstrates the program by sending messages with different priorities.
Adjust the sleep duration to allow threads to execute based on your system's performance.
Test Cases
Multiple Senders, Varying Priorities:

Input: Threads sending messages with a mix of high, medium, and low priorities.
Expected Output: Messages processed in order of priority (highest first), regardless of the order in which they were sent.
Thread Pool Load:

Input: Submit a large number of tasks to the thread pool.
Expected Output: Tasks executed concurrently, demonstrating the thread pool's ability to handle workload.
Additional Notes
Synchronization:

Mutexes (std::mutex) and condition variables (std::condition_variable) are crucial for preventing race conditions and ensuring correct signaling between threads interacting with the message queue.
Code Style:

Adherence to consistent coding conventions improves readability and maintainability. Consider using a linter or code formatter to enforce style guidelines
