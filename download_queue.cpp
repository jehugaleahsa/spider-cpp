#include "download_queue.hpp"
#include "url.hpp"

namespace spider {

    DownloadQueue::DownloadQueue() {
    }
    
    void DownloadQueue::addUrl(Url const& url) {
        if (visitedUrls.find(url) == visitedUrls.end()) {
            urls.push(url);
            visitedUrls.insert(url);
        }
    }
    
    bool DownloadQueue::hasMore() const {
        return !urls.empty();
    }
    
    Url DownloadQueue::getNextUrl() {
        Url next = urls.front();
        urls.pop();
        return next;
    }

}
