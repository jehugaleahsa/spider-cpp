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
        Counter();

        void increment();

        void decrement();

        void wait();
    };
}

#endif // SPIDER_COUNTER_HPP
