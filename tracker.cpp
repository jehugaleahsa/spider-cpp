#include <mutex>
#include "tracker.hpp"
#include "url.hpp"

spider::UrlTracker::UrlTracker() {
}

bool spider::UrlTracker::addUrl(Url const& url) {
    using std::lock_guard;
    using std::mutex;


    lock_guard<mutex> guard(m_urls_mutex);
    return m_urls.insert(url).second;
}
