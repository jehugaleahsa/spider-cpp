#ifndef SPIDER_THREAD_POOL_HPP
#define SPIDER_THREAD_POOL_HPP

#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <queue>
#include <thread>

namespace spider {

    int getProcessorCount();

    class Consumer {
        std::queue<std::function<void(void)>> & m_tasks;
        std::mutex & m_queue_mutex;
        std::mutex & m_has_tasks_mutex;
        std::shared_ptr<std::thread> m_thread;

        std::function<void(void)> getTask();

        void consume();

    public:
        Consumer(
            std::queue<std::function<void(void)>> & tasks,
            std::mutex & queue_mutex,
            std::mutex & has_tasks_mutex);
        
        void start();
    };

    class ThreadPool {
        std::vector<std::shared_ptr<Consumer>> m_pool;
        std::queue<std::function<void(void)>> m_tasks;
        std::mutex m_queue_mutex;
        std::mutex m_has_tasks_mutex;

        ThreadPool(ThreadPool const& other);
        ThreadPool& operator=(ThreadPool const& other);
        
        std::shared_ptr<Consumer> create();

    public:
        ThreadPool(int size);
        
        void start();

        void addTask(std::function<void(void)> callable);
    };

}

#endif // SPIDER_THREAD_POOL_HPP
