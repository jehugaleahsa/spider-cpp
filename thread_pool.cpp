#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include "counter.hpp"
#include "thread_pool.hpp"

int spider::getProcessorCount() {
    return std::thread::hardware_concurrency();
}

std::function<void(void)> spider::Consumer::getTask() {
    using std::defer_lock;
    using std::function;
    using std::mutex;
    using std::unique_lock;

    unique_lock<mutex> queue_access_lock(m_queue_mutex, defer_lock);
    lock(queue_access_lock, m_has_tasks_mutex);
    function<void(void)> callable = m_tasks.front();
    m_tasks.pop();
    if (!m_tasks.empty()) {
        m_has_tasks_mutex.unlock();
    }
    return callable;
}

void spider::Consumer::consume() {
    using std::function;
    
    while (true) {
        function<void(void)> callable = getTask();
        try {
            callable();
            m_counter.decrement();
        } catch (...) {
            m_counter.decrement();
        }
    }
}

spider::Consumer::Consumer(
    Counter & counter,
    std::queue<std::function<void(void)> > & tasks,
    std::mutex & queue_mutex,
    std::mutex & has_tasks_mutex)
:
    m_counter(counter),
    m_tasks(tasks),
    m_queue_mutex(queue_mutex),
    m_has_tasks_mutex(has_tasks_mutex) {
}

spider::Consumer::~Consumer() {
    if (m_thread != nullptr) {
        m_thread->detach();
    }
}

void spider::Consumer::start() {
    using std::unique_ptr;
    using std::thread;
    
    m_thread = unique_ptr<thread>(new thread([&]() { consume(); }));
}

std::unique_ptr<spider::Consumer> spider::ThreadPool::create() {
    using std::unique_ptr;
    
    return unique_ptr<Consumer>(new Consumer(
        m_counter,
        m_tasks,
        m_queue_mutex,
        m_has_tasks_mutex));
}

spider::ThreadPool::ThreadPool(Counter & counter, int size) 
    : m_counter(counter) {
    using std::back_inserter;
    using std::generate_n;
    
    m_has_tasks_mutex.lock();
    generate_n(back_inserter(m_pool), size, [&]() { return create(); });
}

void spider::ThreadPool::start() {
    using std::for_each;
    
    for_each(
        m_pool.begin(), m_pool.end(), 
        [](std::unique_ptr<Consumer> & consumer) { consumer->start(); });
}

void spider::ThreadPool::addTask(std::function<void(void)> callable) {
    using std::defer_lock;
    using std::lock_guard;
    using std::mutex;
    using std::unique_lock;

    m_counter.increment();
    lock_guard<mutex> queue_access_lock(m_queue_mutex);
    m_tasks.push(callable);
    m_has_tasks_mutex.unlock();
}
