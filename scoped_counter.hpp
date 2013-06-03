#ifndef SPIDER_SCOPED_COUNTER_HPP
#define SPIDER_SCOPED_COUNTER_HPP

#include "counter.hpp"

namespace spider {

    class ScopedCounter {
        Counter & m_counter;

        ScopedCounter(ScopedCounter const& other);

        ScopedCounter & operator=(ScopedCounter const& other);

    public:
        ScopedCounter(Counter & counter);

        ~ScopedCounter();
    };

}

#endif  // SPIDER_SCOPED_COUNTER_HPP
