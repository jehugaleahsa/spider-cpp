#include <mutex>
#include "counter.hpp"

spider::Counter::Counter() 
    : m_counter(0) {
    m_wait_mutex.lock();
}

void spider::Counter::increment() {
    using std::lock_guard;
    using std::mutex;

    lock_guard<mutex> guard(m_counter_mutex);
    ++m_counter;
}

void spider::Counter::decrement() {
    using std::lock_guard;
    using std::mutex;

    lock_guard<mutex> guard(m_counter_mutex);
    if (m_counter == 0 || --m_counter == 0) {
        m_wait_mutex.unlock();
    }
}

void spider::Counter::wait() {
    m_wait_mutex.lock();
}
