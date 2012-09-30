#ifndef SPIDER_DOWNLOAD_QUEUE
#define SPIDER_DOWNLOAD_QUEUE

#include <queue>
#include <boost/unordered_set.hpp>
#include "url.hpp"

namespace spider {

    class UrlContext {
        Url m_referrer;
        Url m_url;
        int m_depth;

    public:
        UrlContext(Url const& referrer, Url const& url, int depth);

        Url const& getReferrer() const;
        Url const& getUrl() const;
        int getDepth() const;
    };

    class DownloadQueue {
        boost::unordered_set<Url> visitedUrls;
        std::queue<UrlContext> urls;

    public:
        DownloadQueue();

        void addUrl(Url const& referrer, Url const& url, int depth);

        bool hasMore() const;

        UrlContext getNextUrl();
    };

}

#endif // SPIDER_DOWNLOAD_QUEUE
