#ifndef SPIDER_PAGE_DOWNLOADER_HPP
#define SPIDER_PAGE_DOWNLOADER_HPP

#include <string>
#include <boost/function.hpp>
#include "categorizer.hpp"
#include "downloader.hpp"
#include "extractor.hpp"
#include "stripper.hpp"
#include "thread_pool.hpp"
#include "tracker.hpp"
#include "url.hpp"

namespace spider {

    class PageDownloader : public virtual Downloader {
        ThreadPool & m_pool;
        UrlTracker & m_tracker;
        Categorizer const& m_pageCategorizer;
        Categorizer const& m_mediaCategorizer;
        Stripper const& m_stripper;
        UrlExtractor const& m_baseExtractor;
        UrlExtractor const& m_extractor;

        HttpRequest::response_ptr getResponse() const;

        std::string getContent(HttpRequest::response_ptr response) const;

        Url getBaseUrl(std::string const& content) const;

        void queuePageDownloads(
            std::vector<Url>::const_iterator begin,
            std::vector<Url>::const_iterator end
        );
        
        void queuePageDownload(Url const& url, bool reuseReferrer);

        void queueFileDownloads(
            std::vector<Url>::const_iterator begin,
            std::vector<Url>::const_iterator end
        );
        
        void queueFileDownload(Url const& url);


    public:
        PageDownloader(
            Counter & counter,
            Url const& url,
            boost::shared_ptr<Url> const referrer,
            ThreadPool & pool,
            UrlTracker & tracker,
            Categorizer const& pageCategorizer,
            Categorizer const& mediaCategorizer,
            Stripper const& stripper,
            UrlExtractor const& baseExtractor,
            UrlExtractor const& extractor
        );

        virtual void download();
    };

}

#endif
