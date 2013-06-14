#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include "counter.hpp"
#include "task_pool.hpp"

namespace {

    int getTimestamp() {
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
    : 
    m_priority(priority), 
    m_timestamp(getTimestamp()), 
    m_task(task) {
}

std::function<void(void)> spider::Consumer::getTask() {
    using std::defer_lock;
    using std::get;
    using std::function;
    using std::mutex;
    using std::tuple;
    using std::unique_lock;

    unique_lock<mutex> queue_access_lock(m_queue_mutex, defer_lock);
    lock(queue_access_lock, m_has_tasks_mutex);
    Task task = m_tasks.top();
    m_tasks.pop();
    if (!m_tasks.empty()) {
        m_has_tasks_mutex.unlock();
    }
    return task.getTask();
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
    std::priority_queue<Task> & tasks,
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

spider::TaskPool::~TaskPool() {
}

spider::SingletonPool::SingletonPool() {
}

void spider::SingletonPool::start() {
}

void spider::SingletonPool::addTask(int priority, std::function<void(void)> callable) {
    m_tasks.push(Task(priority, callable));
    if (!m_inProgress) {
        m_inProgress = true;
        while (!m_tasks.empty()) {
            Task task = m_tasks.top();
            m_tasks.pop();
            task.getTask()();
        }
        m_inProgress = false;
    }
}

void spider::SingletonPool::wait() {
}

std::unique_ptr<spider::Consumer> spider::ThreadPool::create() {
    using std::unique_ptr;
    
    return unique_ptr<Consumer>(new Consumer(
        m_counter,
        m_tasks,
        m_queue_mutex,
        m_has_tasks_mutex));
}

spider::ThreadPool::ThreadPool(int size) 
    : m_counter() {
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

void spider::ThreadPool::addTask(int priority, std::function<void(void)> callable) {
    using std::defer_lock;
    using std::lock_guard;
    using std::make_tuple;
    using std::mutex;
    using std::unique_lock;

    m_counter.increment();
    lock_guard<mutex> queue_access_lock(m_queue_mutex);
    m_tasks.push(Task(priority, callable));
    m_has_tasks_mutex.unlock();
}

void spider::ThreadPool::wait() {
    m_counter.wait();
}
