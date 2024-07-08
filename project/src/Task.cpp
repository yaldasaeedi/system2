#include "Task.h"

Task::Task(const std::string& name, int period, int execution_time,
           const std::vector<int>& resource_requirements, int destination_processor, int repetitions)
    : name(name), period(period), execution_time(execution_time),
      required_resources(resource_requirements), destination_processor(destination_processor), repetitions(repetitions) {
    // Constructor implementation
}

const std::string& Task::getName() const {
    return name;
}
