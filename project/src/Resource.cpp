#include "Resource.h"

Resource::Resource(int id, int quantity) : id(id), quantity(quantity) {}

Resource::Resource(Resource&& other) noexcept : id(other.id), quantity(other.quantity.load()) {
    other.id = 0;
    other.quantity.store(0);
}

Resource& Resource::operator=(Resource&& other) noexcept {
    if (this != &other) {
        id = other.id;
        quantity.store(other.quantity.load());
        other.id = 0;
        other.quantity.store(0);
    }
    return *this;
}
