#ifndef SPIDER_DOWNLOADER_HPP
#define SPIDER_DOWNLOADER_HPP

#include "counter.hpp"
#include "http_request.hpp"
#include "thread_pool.hpp"
#include "url.hpp"

namespace spider {

    class Downloadable {
        Counter & m_counter;
        Url m_url;
        Url m_referrer;

    protected:
        void addReferrerHeader(HttpRequest & request) const;

        void addUserAgentHeader(HttpRequest & request) const;

        void addAcceptHeader(HttpRequest & request) const;

        void addHostHeader(HttpRequest & request) const;

        void addConnectionHeader(HttpRequest & request) const;

        Downloadable(Counter & counter, Url const& url, Url const& referrer);

        Counter & getCounter();

        Url const& getUrl() const;

        Url const& getReferrer() const;

    public:
        Downloadable(Downloadable const& other);

        virtual ~Downloadable();

        virtual void download() = 0;
    };

    class Downloader {
        boost::shared_ptr<Downloadable> m_downloadable;

    public:
        Downloader(boost::shared_ptr<Downloadable> downloadable);

        void operator()();
    };

    Downloader makeDownloader(boost::shared_ptr<Downloadable> downloadable);

}

#endif // end SPIDER_DOWNLOADER_HPP
