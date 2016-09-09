#ifndef SPIDER_TASK_POOL_HPP
#define SPIDER_TASK_POOL_HPP

#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <thread>
#include <utility>
#include <chrono>
#include "queue.hpp"

namespace spider {

    class Task {
        int m_priority;
        std::chrono::milliseconds::rep m_timestamp;
        std::function<void(void)> m_task;

    public:
        Task(int priority, std::function<void(void)> task);

        Task(Task const& other);

        Task & operator=(Task const& other);

        int getPriority() const;

        std::chrono::milliseconds::rep getTimestamp() const;

        void run() const;

        friend bool operator <(Task const& first, Task const& second);
    };

    inline int Task::getPriority() const {
        return m_priority;
    }

    inline std::chrono::milliseconds::rep Task::getTimestamp() const {
        return m_timestamp;
    }

    inline void Task::run() const {
        m_task();
    }

    inline bool operator<(Task const& first, Task const& second) {
        using std::make_tuple;

        return make_tuple(first.m_priority, first.m_timestamp) < make_tuple(second.m_priority, second.m_timestamp);
    }

    class Consumer {
    private:
        ProducerConsumerQueue<Task> & m_tasks;
        std::unique_ptr<std::thread> m_thread;

        void consume();

    public:
        Consumer(ProducerConsumerQueue<Task> & tasks);
        
        void start();

        void stop();
    };

    class TaskPool {
    public:
        virtual ~TaskPool();

        virtual void start() = 0;

        virtual void addTask(int priority, std::function<void(void)> callable) = 0;

        virtual void wait() = 0;
    };

    class SingletonPool : public virtual TaskPool {

        SingletonPool(SingletonPool const& other);
        SingletonPool & operator=(SingletonPool const& other);

    public:
        SingletonPool();

        void start();

        void addTask(int priority, std::function<void(void)> callable);

        void wait();
    };

    class ThreadPool : public virtual TaskPool {
        std::vector<std::unique_ptr<Consumer>> m_pool;
        ProducerConsumerQueue<Task> m_tasks;

        ThreadPool(ThreadPool const& other);
        ThreadPool & operator=(ThreadPool const& other);
        
        std::unique_ptr<Consumer> create();

    public:
        ThreadPool(int size);
        
        void start();

        void addTask(int priority, std::function<void(void)> callable);

        void wait();
    };

}

#endif // SPIDER_TASK_POOL_HPP
