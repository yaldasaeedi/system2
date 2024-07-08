#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <functional>
#include <queue>
#include <thread> // Include <thread> for std::thread
#include "Task.h" // Forward declare or include Task here

class RealTimeSystem; // Forward declare RealTimeSystem if needed

class Processor {
public:
    Processor(int id, std::function<bool(Task, Task)> comparator, RealTimeSystem& system);
    Processor(Processor&& other) noexcept;
    Processor& operator=(Processor&& other) noexcept;

    void start();
    void stop();
    void joinProcessorThread();
    void addTask(const Task& task);
    void runNextTask();
    void printStatus() const;

    int id;
    std::function<bool(Task, Task)> comparator; // Store the comparator function
    std::priority_queue<Task, std::vector<Task>, std::function<bool(Task, Task)>> ready_queue; // Use std::function for the priority_queue
    Task current_task;
    std::atomic<bool> is_busy;
    std::thread processor_thread; // Declare std::thread here
    RealTimeSystem& system;
};

#endif // PROCESSOR_H
