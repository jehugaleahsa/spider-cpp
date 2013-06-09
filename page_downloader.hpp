#ifndef SPIDER_PAGE_DOWNLOADER_HPP
#define SPIDER_PAGE_DOWNLOADER_HPP

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include "downloader.hpp"
#include "download_manager.hpp"
#include "url.hpp"
#include "url_finder.hpp"

namespace spider {

    class PageDownloader : public virtual Downloader {
        DownloadManager & m_manager;
        UrlFinder const& m_finder;

        HttpResponse getResponse() const;

        std::string getContent(HttpResponse & response) const;

        void handleRedirect(HttpResponse & response) const;

    public:
        PageDownloader(
            Url const& url, 
            boost::optional<Url> referrer,
            DownloadManager & manager,
            UrlFinder const& finder);

        void download() const override;
    };

}

#endif  // SPIDER_PAGE_DOWNLOADER_HPP
