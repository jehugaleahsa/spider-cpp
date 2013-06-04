#ifndef SPIDER_PAGE_DOWNLOADER_HPP
#define SPIDER_PAGE_DOWNLOADER_HPP

#include <string>
#include <vector>
#include "categorizer.hpp"
#include "downloader.hpp"
#include "extractor.hpp"
#include "stripper.hpp"
#include "thread_pool.hpp"
#include "tracker.hpp"
#include "url.hpp"

namespace spider {

    class PageDownloader : public virtual Downloader {
        HttpResponse getResponse() const;

        std::string getContent(HttpResponse & response) const;

        void handleRedirect(
            HttpResponse & response,
            std::string const& downloadDirectory,
            ThreadPool & pool,
            UrlTracker & tracker,
            Categorizer const& pageCategorizer,
            Categorizer const& mediaCategorizer,
            Stripper const& stripper,
            UrlExtractor const& baseExtractor,
            UrlExtractor const& extractor) const;

        Url getBaseUrl(UrlExtractor const& baseExtractor, std::string const& content) const;

        void queuePageDownloads(
            std::vector<Url>::const_iterator begin,
            std::vector<Url>::const_iterator end,
            std::string const& downloadDirectory,
            ThreadPool & pool,
            UrlTracker & tracker,
            Categorizer const& pageCategorizer,
            Categorizer const& mediaCategorizer,
            Stripper const& stripper,
            UrlExtractor const& baseExtractor,
            UrlExtractor const& extractor
        ) const;
        
        void queuePageDownload(
            Url const& url, 
            bool reuseReferrer,
            std::string const& downloadDirectory,
            ThreadPool & pool,
            UrlTracker & tracker,
            Categorizer const& pageCategorizer,
            Categorizer const& mediaCategorizer,
            Stripper const& stripper,
            UrlExtractor const& baseExtractor,
            UrlExtractor const& extractor
        ) const;

        void queueFileDownloads(
            std::vector<Url>::const_iterator begin,
            std::vector<Url>::const_iterator end,
            ThreadPool & pool,
            UrlTracker & tracker,
            std::string const& downloadDirectory
        ) const;
        
        void queueFileDownload(
            Url const& url,
            ThreadPool & pool,
            UrlTracker & tracker,
            std::string const& downloadDirectory) const;

    public:
        PageDownloader(Url const& url, Url const& referrer);

        void download(
            std::string const& downloadDirectory,
            ThreadPool & pool,
            UrlTracker & tracker,
            Categorizer const& pageCategorizer,
            Categorizer const& mediaCategorizer,
            Stripper const& stripper,
            UrlExtractor const& baseExtractor,
            UrlExtractor const& extractor) const;
    };

}

#endif  // SPIDER_PAGE_DOWNLOADER_HPP
