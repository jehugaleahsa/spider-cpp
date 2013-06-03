#include "counter.hpp"
#include "scoped_counter.hpp"

spider::ScopedCounter::ScopedCounter(Counter & counter) 
    : m_counter(counter) {
    m_counter.increment();
}

spider::ScopedCounter::~ScopedCounter() {
    m_counter.decrement();
}
