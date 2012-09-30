#ifndef SPIDER_FILE_DOWNLOADER_HPP
#define SPIDER_FILE_DOWNLOADER_HPP

#include <string>
#include "downloader.hpp"
#include "url.hpp"

namespace spider {

    class FileDownloader : public virtual Downloader {
        std::string m_directoryPath;

    public:
        FileDownloader(std::string const& directoryPath);

        void download(Url const& referrer, Url const& url) const;
    };

}

#endif // SPIDER_FILE_DOWNLOADER_HPP
