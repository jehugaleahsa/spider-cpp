#ifndef SPIDER_PAGE_DOWNLOADER_HPP
#define SPIDER_PAGE_DOWNLOADER_HPP

#include <string>
#include <vector>
#include "downloader.hpp"
#include "download_manager.hpp"
#include "url.hpp"
#include "url_finder.hpp"

namespace spider {

    class PageDownloader : public virtual Downloader {
        HttpResponse getResponse() const;

        std::string getContent(HttpResponse & response) const;

        void handleRedirect(
            HttpResponse & response,
            DownloadManager & manager,
            UrlFinder const& finder) const;

    public:
        PageDownloader(Url const& url, Url const& referrer);

        void download(
            DownloadManager & manager,
            UrlFinder const& finder) const;
    };

}

#endif  // SPIDER_PAGE_DOWNLOADER_HPP
