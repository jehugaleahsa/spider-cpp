#include <functional>
#include "download_factory.hpp"
#include "file_downloader.hpp"
#include "file_download_factory.hpp"
#include "url.hpp"

spider::FileDownloadFactory::FileDownloadFactory(std::string const& downloadDirectory) 
    : m_downloadDirectory(downloadDirectory) {
}

std::function<void(void)> spider::FileDownloadFactory::create(
    Url const& url,
    Url const& referrer) const {
    auto action = [&, url, referrer]() {
        FileDownloader downloader(url, referrer);
        downloader.download(m_downloadDirectory);
    };
    return action;
}
