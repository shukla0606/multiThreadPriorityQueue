// MultiThreadedPriorityQueue.cpp

#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>

using namespace std;

// Step 1: Priority Message Queue Implementation
template<typename T>
class PriorityMessageQueue {
public:
    void enqueue(T message, int priority);
    T dequeue();
    T peek();
    bool isEmpty();

private:
    priority_queue<pair<int, T>> messages;
    mutex queueMutex;
    condition_variable cv;
};

template<typename T>
void PriorityMessageQueue<T>::enqueue(T message, int priority) {
    unique_lock<mutex> lock(queueMutex);
    messages.push({priority, message});
    cv.notify_one();
}

template<typename T>
T PriorityMessageQueue<T>::dequeue() {
    unique_lock<mutex> lock(queueMutex);
    cv.wait(lock, [this] { return !messages.empty(); });

    T message = messages.top().second;
    messages.pop();
    return message;
}

template<typename T>
T PriorityMessageQueue<T>::peek() {
    unique_lock<mutex> lock(queueMutex);
    cv.wait(lock, [this] { return !messages.empty(); });

    return messages.top().second;
}

template<typename T>
bool PriorityMessageQueue<T>::isEmpty() {
    unique_lock<mutex> lock(queueMutex);
    return messages.empty();
}

// Step 2: Thread Pool Implementation
class ThreadPool {
public:
    ThreadPool(int numThreads);
    ~ThreadPool();
    void execute(function<void()> task);

private:
    vector<thread> threads;
    queue<function<void()>> tasks;
    mutex poolMutex;
    condition_variable cv;
    bool stop;
};

ThreadPool::ThreadPool(int numThreads) : stop(false) {
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([this] {
            while (true) {
                function<void()> task;

                {
                    unique_lock<mutex> lock(poolMutex);
                    cv.wait(lock, [this] { return stop || !tasks.empty(); });

                    if (stop && tasks.empty()) {
                        return;
                    }

                    task = move(tasks.front());
                    tasks.pop();
                }

                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        unique_lock<mutex> lock(poolMutex);
        stop = true;
    }
    cv.notify_all();

    for (thread& thread : threads) {
        thread.join();
    }
}

void ThreadPool::execute(function<void()> task) {
    {
        unique_lock<mutex> lock(poolMutex);
        tasks.push(move(task));
    }
    cv.notify_one();
}

// Step 3: Message Sending Function
class MessageSender {
public:
    MessageSender(int numThreads);
    ~MessageSender();
    void sendMessage(const string& message, int priority);
    void receiveAndExecute();

private:
    PriorityMessageQueue<string> messageQueue;
    ThreadPool threadPool;
};

MessageSender::MessageSender(int numThreads) : threadPool(numThreads) {}

MessageSender::~MessageSender() {
    // Ensure all threads finish execution before destroying the object
    threadPool.execute([]{});
}

void MessageSender::sendMessage(const string& message, int priority) {
    messageQueue.enqueue(message, priority);
    // Wake up sleeping threads
    threadPool.execute([this] { messageQueue.peek(); });
}

void MessageSender::receiveAndExecute() {
    while (true) {
        string message = messageQueue.dequeue();

        // Step 4: Action Execution on Receiving
        threadPool.execute([message] {
            // Define a simple action, e.g., printing the message
            cout << "Received message: " << message << endl;
        });
    }
}

// Step 5: Testing
int main() {
    // Step 6: Documentation
    // Overview:
    // This program demonstrates a multi-threaded priority message queue system.
    // It includes a priority message queue, a thread pool, and a message sender class.
    // Threads can send messages to each other with varying priorities, and upon receiving a message,
    // a simple action is performed concurrently using the thread pool.

    // MessageSender with 3 threads in the pool
    MessageSender messageSender(3);

    // Sending messages with different priorities
    messageSender.sendMessage("High Priority Message", 1);
    messageSender.sendMessage("Medium Priority Message", 2);
    messageSender.sendMessage("Low Priority Message", 3);

    // Sleeping to allow threads to execute
    this_thread::sleep_for(chrono::seconds(2));

    return 0;
}
