#ifndef SPIDER_DOWNLOADER_HPP
#define SPIDER_DOWNLOADER_HPP

#include <boost/shared_ptr.hpp>
#include "counter.hpp"
#include "http_request.hpp"
#include "url.hpp"

namespace spider {

    class Downloader {
        Counter & m_counter;
        Url m_url;
        boost::shared_ptr<Url> m_referrer;
        
        Downloader(Downloader const& other);
        Downloader & operator=(Downloader const& downloadable);

    protected:
        void addReferrerHeader(HttpRequest & request) const;

        void addUserAgentHeader(HttpRequest & request) const;

        void addAcceptHeader(HttpRequest & request) const;

        void addHostHeader(HttpRequest & request) const;

        void addConnectionHeader(HttpRequest & request) const;

        Downloader(
            Counter & counter,
            Url const& url, 
            boost::shared_ptr<Url> const referrer);

        Counter & getCounter();

        Url const& getUrl() const;

        boost::shared_ptr<Url> const getReferrer() const;

    public:
        virtual ~Downloader();

        virtual void download() = 0;
    };

}

#endif // end SPIDER_DOWNLOADER_HPP
