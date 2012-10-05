#include <boost/thread.hpp>
#include "tracker.hpp"
#include "url.hpp"

namespace spider {

    UrlTracker::UrlTracker() {
    }

    bool UrlTracker::addUrl(Url const& url) {
        using boost::lock_guard;
        using boost::mutex;
        using boost::unordered_set;


        lock_guard<mutex> guard(m_urls_mutex);
        return m_urls.insert(url).second;
    }

}
