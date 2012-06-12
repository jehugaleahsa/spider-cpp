#ifndef SPIDER_DOWNLOAD_QUEUE
#define SPIDER_DOWNLOAD_QUEUE

#include <queue>
#include <boost/unordered_set.hpp>
#include "url.hpp"

namespace spider {

class DownloadQueue {

    boost::unordered_set<Url> visitedUrls;
    std::queue<Url> urls;
    
public:
    DownloadQueue();
    
    void addUrl(Url const& url);
    
    bool hasMore() const;
    
    Url getNextUrl();
};

}

#endif // SPIDER_DOWNLOAD_QUEUE
