#ifndef QUEUE_H
#define QUEUE_H

#include "Task.h"
#include <queue>
#include <functional>

class Queue {
public:
    Queue(std::function<bool(Task, Task)> comparator);
    std::priority_queue<Task, std::vector<Task>, std::function<bool(Task, Task)>> tasks;
};

#endif // QUEUE_H
