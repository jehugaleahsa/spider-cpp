#include <iostream>
#include "spider.hpp"
#include "url.hpp"

// #include <iostream>
// #include <cstdlib>
// #include <boost/thread.hpp>
// #include "counter.hpp"
// #include "thread_pool.hpp"

std::string getStartingUrl() {
    using std::cin;
    using std::cout;
    using std::string;

    cout << "Please enter the URL: ";
    string urlString;
    cin >> urlString;
    return urlString;
}

// class Spawner {
//     spider::Counter & m_counter;
//     spider::ThreadPool<Spawner> & m_pool;
//     int m_depth;

// public:
//     Spawner(
//         spider::Counter & counter,
//         spider::ThreadPool<Spawner> & pool,
//         int depth
//     ) : m_counter(counter), m_pool(pool), m_depth(depth) {
//         m_counter.increment();
//     }

//     Spawner(Spawner const& other)
//         :
//         m_counter(other.m_counter),
//         m_pool(other.m_pool),
//         m_depth(other.m_depth) {
//             m_counter.increment();
//     }

//     ~Spawner() {
//         m_counter.decrement();
//     }

//     virtual void operator()() {
//         if (m_depth > 0) {
//             for (int i = 0; i != 10; ++i) {
//                 Spawner spawner(m_counter, m_pool, m_depth - 1);
//                 m_pool.addTask(spawner);
//             }
//         }
//         std::cerr << "depth: " << m_depth << std::endl;
//     }
// };

int main(int argc, char** argv) {
    using std::cout;
    using std::string;
    using spider::Spider;
    using spider::Url;

    string topUrlString = (argc < 2) ? getStartingUrl() : argv[1];
    Url topUrl = Url::parse(topUrlString);
    Spider spider;
    spider.run(cout, topUrl);

    // using spider::Counter;
    // using spider::ThreadPool;

    // ThreadPool<Spawner> pool(10);

    // Counter counter;
    // {
    //     const int depth = 5;
    //     Spawner spawner(counter, pool, depth);
    //     pool.addTask(spawner);
    // }

    // counter.wait();

    return 0;
}
