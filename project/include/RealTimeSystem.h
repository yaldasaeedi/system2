#ifndef REALTIMESYSTEM_H
#define REALTIMESYSTEM_H

#include "Queue.h"
#include "Processor.h"
#include "Task.h"
#include "Resource.h"

#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
class Processor;
class RealTimeSystem {
public:

    RealTimeSystem(int num_processors, std::function<bool(Task, Task)> comparator);
    void addResource(int id, int quantity);
    void addTask(const Task& task);
    void start();
    void stop();
    void printSystemState();

    std::vector<Processor> processors;
    std::vector<Resource> resources;
    Queue common_waiting_queue;
    std::mutex mtx;
    std::mutex resource_mutex;
    std::condition_variable cv;
    std::thread main_thread;

    void manageSystem();
    bool allocateResources(const Task& task);
    void releaseResources(const Task& task);
    void assignTaskToProcessor(const Task& task);

private:
    friend class Processor; // Allow Processor to access private members
};

#endif // REALTIMESYSTEM_H
