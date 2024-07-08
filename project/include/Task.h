#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

struct Task {
    std::string name;
    int period;
    int execution_time;
    std::vector<int> required_resources;
    int destination_processor;
    int repetitions;

    Task(const std::string& name, int period, int execution_time,
         const std::vector<int>& resource_requirements, int destination_processor, int repetitions);
    const std::string& getName() const;
};

#endif // TASK_H
