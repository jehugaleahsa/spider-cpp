#ifndef SPIDER_FILE_DOWNLOAD_FACTORY_HPP
#define SPIDER_FILE_DOWNLOAD_FACTORY_HPP

#include <functional>
#include <string>
#include "download_factory.hpp"
#include "url.hpp"

namespace spider {

    class FileDownloadFactory : public virtual DownloadFactory {
        std::string m_downloadDirectory;

    public:
        FileDownloadFactory(std::string const& downloadDirectory);

        std::function<void(void)> create(Url const& url, Url const& referrer) const;
    };

}

#endif  // SPIDER_FILE_DOWNLOAD_FACTORY_HPP
