#ifndef SPIDER_TRACKER_HPP
#define SPIDER_TRACKER_HPP

#include <mutex>
#include <unordered_set>
#include "url.hpp"

namespace spider {

    class UrlTracker {
        std::mutex m_urls_mutex;
        std::unordered_set<Url> m_urls;

    public:
        UrlTracker();

        bool addUrl(Url const& url);
    };

}

#endif
