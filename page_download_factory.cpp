#include <functional>
#include <boost/optional.hpp>
#include "download_manager.hpp"
#include "page_downloader.hpp"
#include "page_download_factory.hpp"
#include "url.hpp"
#include "url_finder.hpp"

spider::PageDownloadFactory::PageDownloadFactory(
    DownloadManager & manager,
    UrlFinder const& finder) 
    : m_manager(manager), m_finder(finder) {
}

std::function<void(void)> spider::PageDownloadFactory::create(
    Url const& url,
    boost::optional<Url> referrer) const {
    auto action = [&, url, referrer]() {
        PageDownloader downloader(url, referrer);
        downloader.download(m_manager, m_finder);
    };
    return action;
}
