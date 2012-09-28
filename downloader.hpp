#ifndef SPIDER_DOWNLOADER_HPP
#define SPIDER_DOWNLOADER_HPP

#include "http_request.hpp"
#include "url.hpp"

namespace spider {

class Downloader {
protected:
    static void addReferrerHeader(HttpRequest & request, Url const& url);
    static void addUserAgentHeader(HttpRequest & request);
    static void addAcceptHeader(HttpRequest & request);
    static void addHostHeader(HttpRequest & request, Url const& url);

    Downloader();

public:
    virtual ~Downloader();
};

}

#endif // end SPIDER_DOWNLOADER_HPP
