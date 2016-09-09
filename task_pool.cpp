#include <chrono>
#include <functional>
#include <memory>
#include <queue>
#include <thread>
#include <utility>
#include <boost/optional.hpp>
#include "task_pool.hpp"

namespace {

    std::chrono::milliseconds::rep getTimestamp() {
        using std::chrono::duration;
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
        using std::chrono::time_point;
        using std::chrono::system_clock;

        time_point<system_clock> now = system_clock::now();
        system_clock::duration elapsed = now.time_since_epoch();
        return duration_cast<milliseconds>(elapsed).count();
    }

}

spider::Task::Task(int priority, std::function<void(void)> task)
    : m_priority(priority), 
    m_timestamp(getTimestamp()), 
    m_task(task) {
}

spider::Task::Task(spider::Task const& other)
    : m_priority(other.m_priority),
    m_timestamp(other.m_timestamp),
    m_task(other.m_task) {
}

spider::Task & spider::Task::operator=(spider::Task const& other) {
    m_priority = other.m_priority;
    m_timestamp = other.m_timestamp;
    m_task = other.m_task;
    return *this;
}

void spider::Consumer::consume() {
    using boost::optional;

    bool hasMore = true;
    while (hasMore) {
        optional<Task> optTask = m_tasks.tryGetItem();
        if (!optTask) {
            hasMore = false;
        } else {
            try {
                Task & task = optTask.value();
                task.run();
                m_tasks.completedItem();
            } catch (...) {
            }
        }
    }
}

spider::Consumer::Consumer(ProducerConsumerQueue<Task> & tasks)
    : m_tasks(tasks), m_thread(nullptr) {
}

void spider::Consumer::start() {
    using std::make_unique;
    using std::thread;
    
    m_thread = make_unique<thread>([&]() { 
        consume(); 
    });
}

void spider::Consumer::stop() {
    m_thread->join();
}

spider::TaskPool::~TaskPool() {
}

spider::SingletonPool::SingletonPool() {
}

void spider::SingletonPool::start() {
}

void spider::SingletonPool::addTask(int priority, std::function<void(void)> callable) {
    callable();
}

void spider::SingletonPool::wait() {
}

std::unique_ptr<spider::Consumer> spider::ThreadPool::create() {
    using std::make_unique;
    
    return make_unique<Consumer>(m_tasks);
}

spider::ThreadPool::ThreadPool(int size) 
    : m_pool(), m_tasks() {
    using std::back_inserter;
    using std::generate_n;
    
    generate_n(back_inserter(m_pool), size, [&]() { 
        return create(); 
    });
}

void spider::ThreadPool::start() {
    using std::for_each;
    using std::begin;
    using std::end;
    
    for_each(begin(m_pool), end(m_pool), [](auto& consumer) { 
        consumer->start(); 
    });
}

void spider::ThreadPool::addTask(int priority, std::function<void(void)> callable) {
    m_tasks.addItem(Task(priority, callable));
}

void spider::ThreadPool::wait() {
    using std::for_each;
    using std::begin;
    using std::end;

    for_each(begin(m_pool), end(m_pool), [](auto& consumer) {
        consumer->stop();
    });
}
