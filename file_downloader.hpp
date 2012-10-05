#ifndef SPIDER_FILE_DOWNLOADER_HPP
#define SPIDER_FILE_DOWNLOADER_HPP

#include <string>
#include "downloader.hpp"
#include "url.hpp"

namespace spider {

    class FileDownloadable : public virtual Downloadable {
        std::string m_directoryPath;

    public:
        FileDownloadable(
            Counter & counter,
            Url const& url,
            Url const& referrer,
            std::string const& directoryPath
        );

        virtual void download();
    };

}

#endif // SPIDER_FILE_DOWNLOADER_HPP
