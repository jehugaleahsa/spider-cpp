#ifndef SPIDER_FILE_DOWNLOADER_HPP
#define SPIDER_FILE_DOWNLOADER_HPP

#include <string>
#include "downloader.hpp"
#include "url.hpp"

namespace spider {

    class FileDownloader : public virtual Downloader {
        std::string m_directoryPath;
        
        FileDownloader(FileDownloader const& other);
        FileDownloader & operator=(FileDownloader const& other);

    public:
        FileDownloader(
            Counter & counter,
            Url const& url,
            Url const& referrer,
            std::string const& directoryPath
        );

        virtual void download();
    };

}

#endif // SPIDER_FILE_DOWNLOADER_HPP
