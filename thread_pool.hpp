#ifndef SPIDER_THREAD_POOL_HPP
#define SPIDER_THREAD_POOL_HPP

#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <queue>
#include <thread>
#include <utility>
#include "counter.hpp"

namespace spider {

    int getProcessorCount();

    class Task {
        int m_priority;
        int m_timestamp;
        std::function<void(void)> m_task;

    public:
        Task(int priority, std::function<void(void)> task);

        int getPriority() const;

        int getTimestamp() const;

        std::function<void(void)> getTask() const;

        friend bool operator <(Task const& first, Task const& second);
    };

    inline int Task::getPriority() const {
        return m_priority;
    }

    inline int Task::getTimestamp() const {
        return m_timestamp;
    }

    inline std::function<void(void)> Task::getTask() const {
        return m_task;
    }

    inline bool operator<(Task const& first, Task const& second) {
        using std::make_tuple;

        return make_tuple(first.m_priority, first.m_timestamp) < make_tuple(second.m_priority, second.m_timestamp);
    }

    class Consumer {
    private:
        Counter & m_counter;
        std::priority_queue<Task> & m_tasks;
        std::mutex & m_queue_mutex;
        std::mutex & m_has_tasks_mutex;
        std::unique_ptr<std::thread> m_thread;

        std::function<void(void)> getTask();

        void consume();

    public:
        Consumer(
            Counter & counter,
            std::priority_queue<Task> & tasks,
            std::mutex & queue_mutex,
            std::mutex & has_tasks_mutex);

        ~Consumer();
        
        void start();
    };

    class ThreadPool {
        Counter & m_counter;
        std::vector<std::unique_ptr<Consumer>> m_pool;
        std::priority_queue<Task> m_tasks;
        std::mutex m_queue_mutex;
        std::mutex m_has_tasks_mutex;

        ThreadPool(ThreadPool const& other);
        ThreadPool& operator=(ThreadPool const& other);
        
        std::unique_ptr<Consumer> create();

    public:
        ThreadPool(Counter & counter, int size);
        
        void start();

        void addTask(int priority, std::function<void(void)> callable);
    };

}

#endif // SPIDER_THREAD_POOL_HPP
