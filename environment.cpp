#include <thread>
#include "environment.hpp"

int spider::Environment::getProcessorCount() {
    return std::thread::hardware_concurrency();
}
