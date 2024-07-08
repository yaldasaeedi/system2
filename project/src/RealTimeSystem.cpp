#include "RealTimeSystem.h"
#include "Processor.h"

#include <algorithm>
#include <iostream>

RealTimeSystem::RealTimeSystem(int num_processors, std::function<bool(Task, Task)> comparator)
    : common_waiting_queue(comparator) {
    for (int i = 0; i < num_processors; ++i) {
        processors.emplace_back(i, comparator, *this);
    }
}

void RealTimeSystem::addResource(int id, int quantity) {
    resources.emplace_back(Resource(id, quantity));
}

void RealTimeSystem::addTask(const Task& task) {
    std::unique_lock<std::mutex> lock(mtx);
    common_waiting_queue.tasks.push(task);
    cv.notify_all();
}

void RealTimeSystem::start() {
    for (auto& processor : processors) {
        processor.start();
    }
    main_thread = std::thread(&RealTimeSystem::manageSystem, this);
}

void RealTimeSystem::stop() {
    for (auto& processor : processors) {
        processor.stop();
    }
    main_thread.join();
    for (auto& processor : processors) {
        processor.joinProcessorThread();
    }
}

void RealTimeSystem::manageSystem() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !common_waiting_queue.tasks.empty(); });

        while (!common_waiting_queue.tasks.empty()) {
            Task task = common_waiting_queue.tasks.top();
            common_waiting_queue.tasks.pop();

            bool allocated = allocateResources(task);
            if (allocated) {
                assignTaskToProcessor(task);
            } else {
                common_waiting_queue.tasks.push(task);
                cv.wait_for(lock, std::chrono::milliseconds(100));
            }
        }
    }
}

bool RealTimeSystem::allocateResources(const Task& task) {
    std::unique_lock<std::mutex> lock(resource_mutex);
    for (int res_id : task.required_resources) {
        auto it = std::find_if(resources.begin(), resources.end(), [&res_id](const Resource& res) {
            return res.id == res_id;
        });
        if (it == resources.end() || it->quantity <= 0) {
            return false;
        }
    }
    for (int res_id : task.required_resources) {
        auto it = std::find_if(resources.begin(), resources.end(), [&res_id](const Resource& res) {
            return res.id == res_id;
        });
        --it->quantity;
    }
    return true;
}

void RealTimeSystem::releaseResources(const Task& task) {
    std::unique_lock<std::mutex> lock(resource_mutex);
    for (int res_id : task.required_resources) {
        auto it = std::find_if(resources.begin(), resources.end(), [&res_id](const Resource& res) {
            return res.id == res_id;
        });
        ++it->quantity;
    }
}

void RealTimeSystem::assignTaskToProcessor(const Task& task) {
    auto it = std::min_element(processors.begin(), processors.end(), [](const Processor& a, const Processor& b) {
        return a.ready_queue.size() < b.ready_queue.size();
    });
    it->addTask(task);
}

void RealTimeSystem::printSystemState() {
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Resources available: ";
    for (const auto& resource : resources) {
        std::cout << "R" << resource.id << ":" << resource.quantity.load() << " ";
    }
    std::cout << std::endl;
    std::cout << "Tasks in Waiting Queue: ";
    auto waiting_queue = common_waiting_queue.tasks;
    while (!waiting_queue.empty()) {
        const auto& task = waiting_queue.top();
        std::cout << task.name << " ";
        waiting_queue.pop();
    }
    for (const auto& processor : processors) {
        processor.printStatus(); // Add this line
        // Print other details about the processor
    }
    std::cout << std::endl;
    for (const auto& processor : processors) {
        std::cout << "Processor " << processor.id << " Utilization: " << processor.ready_queue.size() << std::endl;
        std::cout << "Ready Queue: ";
        auto ready_queue = processor.ready_queue;
        while (!ready_queue.empty()) {
            const auto& task = ready_queue.top();
            std::cout << task.name << "(" << task.repetitions << ") ";
            ready_queue.pop();
        }
        std::cout << std::endl;
        std::cout << "Running Task: " << processor.current_task.getName() << std::endl;
    }
}
