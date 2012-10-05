#ifndef SPIDER_PAGE_DOWNLOADER_HPP
#define SPIDER_PAGE_DOWNLOADER_HPP

#include <string>
#include "categorizer.hpp"
#include "downloader.hpp"
#include "extractor.hpp"
#include "stripper.hpp"
#include "tracker.hpp"
#include "url.hpp"

namespace spider {

    class PageDownloadable : public virtual Downloadable {
        ThreadPool<Downloader> & m_pool;
        UrlTracker & m_tracker;
        Categorizer const& m_pageCategorizer;
        Categorizer const& m_mediaCategorizer;
        Stripper const& m_stripper;
        UrlExtractor const& m_baseExtractor;
        UrlExtractor const& m_anchorExtractor;
        UrlExtractor const& m_imageExtractor;

        std::string getContent() const;

        Url getBaseUrl(std::string const& content) const;

        void queuePageDownloads(
            std::vector<Url>::const_iterator begin,
            std::vector<Url>::const_iterator end
        );

        void queueFileDownloads(
            std::vector<Url>::const_iterator begin,
            std::vector<Url>::const_iterator end
        );


    public:
        PageDownloadable(
            Counter & counter,
            Url const& url,
            Url const& referrer,
            ThreadPool<Downloader> & pool,
            UrlTracker & tracker,
            Categorizer const& pageCategorizer,
            Categorizer const& mediaCategorizer,
            Stripper const& stripper,
            UrlExtractor const& baseExtractor,
            UrlExtractor const& anchorExtractor,
            UrlExtractor const& imageExtractor
        );

        virtual void download();
    };

}

#endif
