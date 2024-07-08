#ifndef RESOURCE_H
#define RESOURCE_H

#include <atomic>

class Resource {
public:
    Resource(int id, int quantity);
    Resource(const Resource&) = delete; // Delete copy constructor
    Resource& operator=(const Resource&) = delete; // Delete copy assignment operator
    Resource(Resource&& other) noexcept; // Move constructor
    Resource& operator=(Resource&& other) noexcept; // Move assignment operator

    int id;
    std::atomic<int> quantity;
};

#endif // RESOURCE_H
