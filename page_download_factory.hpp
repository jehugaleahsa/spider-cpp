#ifndef SPIDER_PAGE_DOWNLOAD_FACTORY_HPP
#define SPIDER_PAGE_DOWNLOAD_FACTORY_HPP

#include <functional>
#include <boost/optional.hpp>
#include "download_factory.hpp"
#include "download_manager.hpp"
#include "url.hpp"
#include "url_finder.hpp"

namespace spider {

    class PageDownloadFactory : public virtual DownloadFactory {
        DownloadManager & m_manager;
        UrlFinder const& m_finder;

    public:
        PageDownloadFactory(DownloadManager & manager, UrlFinder const& finder);

        std::function<void(void)> create(
            Url const& url, 
            boost::optional<Url> referrer) const;
    };

}

#endif  // SPIDER_PAGE_DOWNLOAD_FACTORY_HPP
