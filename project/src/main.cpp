#include <iostream>
#include "RealTimeSystem.h"

auto rateMonotonicComparator = [](const Task& a, const Task& b) {
    return a.period > b.period; // Higher priority for shorter periods
};

// auto earliestDeadlineFirstComparator = [](const Task& a, const Task& b) {
//     return a.deadline > b.deadline; // Higher priority for earlier deadlines
// };

int main() {
    auto comparator = rateMonotonicComparator; // or earliestDeadlineFirstComparator

    RealTimeSystem system(3, comparator);
    system.addResource(3, 5);
    system.addResource(2, 4);
    system.addResource(1, 3);

    // Example input tasks
    system.addTask(Task("T1", 50, 200, {2}, 1, 4));
    system.addTask(Task("T2", 75, 300, {1}, 2, 3));
     system.addTask(Task("T3", 100, 560, {2, 0}, 3, 2));
     system.addTask(Task("T4", 125, 466, {1, 0}, 1, 3));
     system.addTask(Task("T5", 70, 200, {2, 0}, 1, 4));
    system.addTask(Task("T6", 80, 300, {1}, 2, 3));
     system.addTask(Task("T7", 10, 555, {2, 1}, 3, 2));
     system.addTask(Task("T8", 15, 455, {1, 2}, 1, 3));
     system.addTask(Task("T9", 28, 200, {2, 1}, 1, 4));
    system.addTask(Task("T10", 65, 300, {1, 0}, 2, 3));
     system.addTask(Task("T11", 140, 522, {2, 0}, 3, 2));
     system.addTask(Task("T12", 85, 423, {1, 0}, 1, 3));

    system.start();

    // Simulate the passage of time and periodically print the system state
    for (int time = 0; time < 15; ++time) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        system.printSystemState();
    }

    system.stop();

    return 0;
}
