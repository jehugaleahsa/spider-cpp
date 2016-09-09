#ifndef SPIDER_QUEUE_HPP
#define SPIDER_QUEUE_HPP

#include <atomic>
#include <boost/thread/concurrent_queues/sync_priority_queue.hpp>
#include <boost/optional.hpp>

namespace spider {

    template<typename TItem> 
    class ProducerConsumerQueue {
        boost::sync_priority_queue<TItem> m_queue;
        std::atomic_int m_count;

    public:
        ProducerConsumerQueue();

        ProducerConsumerQueue(ProducerConsumerQueue const& other) = delete;

        ProducerConsumerQueue & operator=(ProducerConsumerQueue const& other) = delete;

        void addItem(TItem item);

        void completedItem();

        boost::optional<TItem> tryGetItem();
    };

}

template<typename TItem>
spider::ProducerConsumerQueue<TItem>::ProducerConsumerQueue()
    : m_queue(), m_count(0) {
}

template<typename TItem>
void spider::ProducerConsumerQueue<TItem>::addItem(TItem item) {
    ++m_count;
    m_queue.push(item);
}

template<typename TItem>
void spider::ProducerConsumerQueue<TItem>::completedItem() {
    if (--m_count == 0) {
        m_queue.close();
    }
}

template<typename TItem>
boost::optional<TItem> spider::ProducerConsumerQueue<TItem>::tryGetItem() {
    using boost::sync_queue_is_closed;
    using boost::optional;

    try {
        return m_queue.pull();
    } catch (sync_queue_is_closed) {
        return optional<TItem>();
    }
}

#endif // SPIDER_QUEUE_HPP