#include "Processor.h"
#include "RealTimeSystem.h"
#include <iostream>
#include <thread>

Processor::Processor(int id, std::function<bool(Task, Task)> comparator, RealTimeSystem& system)
    : id(id), comparator(comparator), ready_queue(comparator), current_task("", 0, 0, {}, 0, 0), is_busy(false), system(system) {}

Processor::Processor(Processor&& other) noexcept
    : id(other.id), comparator(std::move(other.comparator)), ready_queue(std::move(other.ready_queue)),
      current_task(std::move(other.current_task)), is_busy(other.is_busy.load()), processor_thread(std::move(other.processor_thread)),
      system(other.system) {}
      
Processor& Processor::operator=(Processor&& other) noexcept {
    if (this != &other) {
        id = other.id;
        comparator = std::move(other.comparator);
        ready_queue = std::move(other.ready_queue);
        current_task = std::move(other.current_task);
        is_busy.store(other.is_busy.load());
        processor_thread = std::move(other.processor_thread);
        // Avoid assigning RealTimeSystem directly, consider if this is necessary in your design
        // system = other.system; // This line causes the error
    }
    return *this;
}


void Processor::start() {
    processor_thread = std::thread([this]() {
        while (true) {
            runNextTask();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

void Processor::stop() {
    if (processor_thread.joinable()) {
        processor_thread.join();
    }
}

void Processor::joinProcessorThread() {
    if (processor_thread.joinable()) {
        processor_thread.join();
    }
}

void Processor::addTask(const Task& task) {
    ready_queue.push(task);
}

void Processor::runNextTask() {
    if (!ready_queue.empty()) {
        current_task = ready_queue.top();
        ready_queue.pop();
        is_busy = true;
        std::cout << "Processor " << id << " is running task " << current_task.getName() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(current_task.execution_time));
        system.releaseResources(current_task);
        is_busy = false;
    }
}

void Processor::printStatus() const {
    std::cout << "Processor " << id << (is_busy ? " is busy with task " : " is idle") << std::endl;
    if (is_busy) {
        std::cout << "Running task: " << current_task.getName() << std::endl;
    }
}
