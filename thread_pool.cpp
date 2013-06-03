#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
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
        } catch (...) {
        }
    }
}

spider::Consumer::Consumer(
    std::queue<std::function<void(void)> > & tasks,
    std::mutex & queue_mutex,
    std::mutex & has_tasks_mutex)
:
    m_tasks(tasks),
    m_queue_mutex(queue_mutex),
    m_has_tasks_mutex(has_tasks_mutex) {
}

void spider::Consumer::start() {
    using std::bind;
    using std::function;
    using std::shared_ptr;
    using std::thread;
    
    m_thread = shared_ptr<thread>(new thread(bind(&Consumer::consume, this)));
}

std::shared_ptr<spider::Consumer> spider::ThreadPool::create() {
    using std::shared_ptr;
    
    return shared_ptr<Consumer>(new Consumer(
        m_tasks,
        m_queue_mutex,
        m_has_tasks_mutex));
}

spider::ThreadPool::ThreadPool(int size) {
    using std::back_inserter;
    using std::bind;
    using std::generate_n;
    using std::ref;
    using std::shared_ptr;
    
    m_has_tasks_mutex.lock();
    generate_n(
        back_inserter(m_pool),
        size,
        bind(&ThreadPool::create, this));
}

void spider::ThreadPool::start() {
    using std::for_each;
    using std::bind;
    using std::placeholders::_1;
    
    for_each(
        m_pool.begin(), m_pool.end(), 
        bind(&Consumer::start, _1));
}

void spider::ThreadPool::addTask(std::function<void(void)> callable) {
    using std::defer_lock;
    using std::lock_guard;
    using std::mutex;
    using std::unique_lock;

    lock_guard<mutex> queue_access_lock(m_queue_mutex);
    m_tasks.push(callable);
    m_has_tasks_mutex.unlock();
}
