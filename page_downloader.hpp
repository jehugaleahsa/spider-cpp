#ifndef SPIDER_PAGE_DOWNLOADER_HPP
#define SPIDER_PAGE_DOWNLOADER_HPP

#include <string>
#include <vector>
#include "categorizer.hpp"
#include "downloader.hpp"
#include "thread_pool.hpp"
#include "tracker.hpp"
#include "url.hpp"
#include "url_finder.hpp"

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
            UrlFinder const& finder) const;

        void queuePageDownloads(
            std::vector<Url>::const_iterator begin,
            std::vector<Url>::const_iterator end,
            std::string const& downloadDirectory,
            ThreadPool & pool,
            UrlTracker & tracker,
            Categorizer const& pageCategorizer,
            Categorizer const& mediaCategorizer,
            UrlFinder const& finder) const;
        
        void queuePageDownload(
            Url const& url, 
            bool reuseReferrer,
            std::string const& downloadDirectory,
            ThreadPool & pool,
            UrlTracker & tracker,
            Categorizer const& pageCategorizer,
            Categorizer const& mediaCategorizer,
            UrlFinder const& finder) const;

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
            UrlFinder const& finder) const;
    };

}

#endif  // SPIDER_PAGE_DOWNLOADER_HPP
