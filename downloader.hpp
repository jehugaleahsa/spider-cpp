#ifndef SPIDER_DOWNLOADER_HPP
#define SPIDER_DOWNLOADER_HPP

#include "http_request.hpp"
#include "url.hpp"

namespace spider {

    class Downloader {
        Url m_url;
        Url m_referrer;
        
        Downloader(Downloader const& other);
        Downloader & operator=(Downloader const& downloadable);

    protected:
        void addReferrerHeader(HttpRequest & request) const;

        void addUserAgentHeader(HttpRequest & request) const;

        void addAcceptHeader(HttpRequest & request) const;

        void addHostHeader(HttpRequest & request) const;

        void addConnectionHeader(HttpRequest & request) const;

        Downloader(
            Url const& url, 
            Url const& referrer);

        Url const& getUrl() const;

        Url const& getReferrer() const;

    public:
        virtual ~Downloader();
    };

}

#endif // end SPIDER_DOWNLOADER_HPP
