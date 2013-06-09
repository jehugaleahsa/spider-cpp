#ifndef SPIDER_FILE_DOWNLOADER_HPP
#define SPIDER_FILE_DOWNLOADER_HPP

#include <string>
#include <boost/optional.hpp>
#include "downloader.hpp"
#include "url.hpp"

namespace spider {

    class FileDownloader : public virtual Downloader {
        std::string m_downloadDirectory;

        FileDownloader(FileDownloader const& other);
        FileDownloader & operator=(FileDownloader const& other);

    public:
        FileDownloader(
            Url const& url, 
            boost::optional<Url> referrer,
            std::string const& downloadDirectory);

        void download() const override;
    };

}

#endif // SPIDER_FILE_DOWNLOADER_HPP
