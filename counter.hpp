#ifndef SPIDER_COUNTER_HPP
#define SPIDER_COUNTER_HPP

#include <mutex>

namespace spider {

    class Counter {
        std::mutex m_wait_mutex;
        std::mutex m_counter_mutex;
        int m_counter;

        Counter(Counter const& other);
        Counter& operator=(Counter const& other);

    public:
        Counter() : m_counter(0) {
            m_wait_mutex.lock();
        }

        void increment() {
            using std::lock_guard;
            using std::mutex;

            lock_guard<mutex> guard(m_counter_mutex);
            ++m_counter;
        }

        void decrement() {
            using std::lock_guard;
            using std::mutex;

            lock_guard<mutex> guard(m_counter_mutex);
            if (m_counter == 0 || --m_counter == 0) {
                m_wait_mutex.unlock();
            }
        }

        void wait() {
            m_wait_mutex.lock();
        }
    };
}

#endif // SPIDER_COUNTER_HPP
