#ifndef SPIDER_FILE_DOWNLOADER_HPP
#define SPIDER_FILE_DOWNLOADER_HPP

#include <string>
#include "downloader.hpp"
#include "url.hpp"

namespace spider {

    class FileDownloader : public virtual Downloader {
        FileDownloader(FileDownloader const& other);
        FileDownloader & operator=(FileDownloader const& other);

    public:
        FileDownloader(Url const& url, Url const& referrer);

        void download(std::string const& downloadDirectory);
    };

}

#endif // SPIDER_FILE_DOWNLOADER_HPP
