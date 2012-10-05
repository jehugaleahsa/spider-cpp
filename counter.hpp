#ifndef SPIDER_COUNTER_HPP
#define SPIDER_COUNTER_HPP

#include <boost/thread.hpp>

namespace spider {

    class Counter {
        boost::mutex m_wait_mutex;
        boost::mutex m_counter_mutex;
        int m_counter;

        Counter(Counter const& other);
        Counter& operator=(Counter const& other);

    public:
        Counter() : m_counter(0) {
            m_wait_mutex.lock();
        }

        void increment() {
            using boost::lock_guard;
            using boost::mutex;

            lock_guard<mutex> guard(m_counter_mutex);
            ++m_counter;
        }

        void decrement() {
            using boost::lock_guard;
            using boost::mutex;

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
