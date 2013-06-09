#include <functional>
#include <boost/optional.hpp>
#include "download_factory.hpp"
#include "file_downloader.hpp"
#include "file_download_factory.hpp"
#include "url.hpp"

spider::FileDownloadFactory::FileDownloadFactory(std::string const& downloadDirectory) 
    : m_downloadDirectory(downloadDirectory) {
}

std::function<void(void)> spider::FileDownloadFactory::create(
    Url const& url,
    boost::optional<Url> referrer) const {
    return [&, url, referrer]() { 
        FileDownloader downloader(url, referrer, m_downloadDirectory);
        downloader.download(); 
    };
}
