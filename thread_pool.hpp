#ifndef SPIDER_THREAD_POOL_HPP
#define SPIDER_THREAD_POOL_HPP

#include <algorithm>
#include <iterator>
#include <queue>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

namespace spider {

    int getProcessorCount();

    class Consumer {
        std::queue<boost::function<void(void)> > & m_tasks;
        boost::mutex & m_queue_mutex;
        boost::mutex & m_has_tasks_mutex;
        boost::shared_ptr<boost::thread> m_thread;

        boost::function<void(void)> getTask();

        void consume();

    public:
        Consumer(
            std::queue<boost::function<void(void)> > & tasks,
            boost::mutex & queue_mutex,
            boost::mutex & has_tasks_mutex);
        
        void start();
    };

    class ThreadPool {
        std::vector<boost::shared_ptr<Consumer> > m_pool;
        std::queue<boost::function<void(void)> > m_tasks;
        boost::mutex m_queue_mutex;
        boost::mutex m_has_tasks_mutex;

        ThreadPool(ThreadPool const& other);
        ThreadPool& operator=(ThreadPool const& other);
        
        boost::shared_ptr<Consumer> create();

    public:
        ThreadPool(int size);
        
        void start();

        void addTask(boost::function<void(void)> callable);
    };

}

#endif // SPIDER_THREAD_POOL_HPP
