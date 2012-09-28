#include "download_queue.hpp"
#include "url.hpp"

namespace spider {

UrlContext::UrlContext(Url const& referrer, Url const& url, int depth)
    : m_referrer(referrer), m_url(url), m_depth(depth) {

}

Url const& UrlContext::getReferrer() const {
    return m_referrer;
}

Url const& UrlContext::getUrl() const {
    return m_url;
}

int UrlContext::getDepth() const {
    return m_depth;
}

DownloadQueue::DownloadQueue() {
}

void DownloadQueue::addUrl(Url const& referrer, Url const& url, int depth) {
    if (visitedUrls.find(url) == visitedUrls.end()) {
        UrlContext context(referrer, url, depth);
        urls.push(context);
        visitedUrls.insert(url);
    }
}

bool DownloadQueue::hasMore() const {
    return !urls.empty();
}

UrlContext DownloadQueue::getNextUrl() {
    UrlContext next = urls.front();
    urls.pop();
    return next;
}

}
