#include "Queue.h"

Queue::Queue(std::function<bool(Task, Task)> comparator) : tasks(comparator) {}
