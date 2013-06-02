#include <queue>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include "thread_pool.hpp"

int spider::getProcessorCount() {
    return boost::thread::hardware_concurrency();
}

boost::function<void(void)> spider::Consumer::getTask() {
    using boost::defer_lock;
    using boost::function;
    using boost::mutex;
    using boost::unique_lock;

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
    using boost::function;
    
    while (true) {
        function<void(void)> callable = getTask();
        try {
            callable();
        } catch (...) {
        }
    }
}

spider::Consumer::Consumer(
    std::queue<boost::function<void(void)> > & tasks,
    boost::mutex & queue_mutex,
    boost::mutex & has_tasks_mutex)
:
    m_tasks(tasks),
    m_queue_mutex(queue_mutex),
    m_has_tasks_mutex(has_tasks_mutex) {
}

void spider::Consumer::start() {
    using boost::bind;
    using boost::function;
    using boost::shared_ptr;
    using boost::thread;
    
    m_thread = shared_ptr<thread>(new thread(bind(&Consumer::consume, this)));
}

boost::shared_ptr<spider::Consumer> spider::ThreadPool::create() {
    using boost::shared_ptr;
    
    return shared_ptr<Consumer>(new Consumer(
        m_tasks,
        m_queue_mutex,
        m_has_tasks_mutex));
}

spider::ThreadPool::ThreadPool(int size) {
    using std::back_inserter;
    using std::generate_n;
    using boost::bind;
    using boost::ref;
    using boost::shared_ptr;
    
    m_has_tasks_mutex.lock();
    generate_n(
        back_inserter(m_pool),
        size,
        bind(&ThreadPool::create, this));
}

void spider::ThreadPool::start() {
    using std::for_each;
    using boost::bind;
    
    for_each(
        m_pool.begin(), m_pool.end(), 
        bind(&Consumer::start, _1));
}

void spider::ThreadPool::addTask(boost::function<void(void)> callable) {
    using boost::defer_lock;
    using boost::lock_guard;
    using boost::mutex;
    using boost::unique_lock;

    lock_guard<mutex> queue_access_lock(m_queue_mutex);
    m_tasks.push(callable);
    m_has_tasks_mutex.unlock();
}
