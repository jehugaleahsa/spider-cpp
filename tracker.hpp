#ifndef SPIDER_TRACKER_HPP
#define SPIDER_TRACKER_HPP

#include <boost/thread.hpp>
#include <boost/unordered_set.hpp>
#include "url.hpp"

namespace spider {

    class UrlTracker {
        boost::mutex m_urls_mutex;
        boost::unordered_set<Url> m_urls;

    public:
        UrlTracker();

        bool addUrl(Url const& url);
    };

}

#endif
