#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "thread_pool.hpp"

namespace spider {

    template <typename TCallable>
    TCallable Consumer<TCallable>::getTask() {
        using boost::defer_lock;
        using boost::mutex;
        using boost::unique_lock;

        unique_lock<mutex> queue_access_lock(m_queue_mutex, defer_lock);
        lock(queue_access_lock, m_has_tasks_mutex);
        TCallable callable = m_tasks.pop();
        if (!m_tasks.empty()) {
            m_has_tasks_mutex.unlock();
        }
        return callable;
    }

    template <typename TCallable>
    Consumer<TCallable>::Consumer(std::queue<TCallable> & tasks,
        boost::mutex & queue_mutex,
        boost::mutex & has_tasks_mutex)
        :
        m_tasks(tasks),
        m_queue_mutex(queue_mutex),
        m_has_tasks_mutex(has_tasks_mutex) {
    }

    template <typename TCallable>
    void Consumer<TCallable>::operator()() {
        while (true) {
            TCallable callable = getTask();
            try {
                callable();
            } catch (...) {
            }
        }
    }

    template <typename TCallable>
    ThreadPool<TCallable>::ThreadPool(int size) {
        using boost::shared_ptr;

        for (int count = 0; count != size; ++count) {
            shared_ptr<Consumer<TCallable> > consumer(new Consumer<TCallable>(
                m_queue_mutex, m_has_tasks_mutex));
            m_pool.push_back(consumer);
        }
    }

    template <typename TCallable>
    void ThreadPool<TCallable>::addTask(TCallable callable) {
        using boost::defer_lock;
        using boost::lock_guard;
        using boost::mutex;
        using boost::unique_lock;

        lock_guard<mutex> queue_access_lock(m_queue_mutex);
        m_tasks.push(callable);
        m_has_tasks_mutex.unlock();
    }
}
