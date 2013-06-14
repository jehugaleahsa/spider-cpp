#ifndef SPIDER_FILE_DOWNLOAD_FACTORY_HPP
#define SPIDER_FILE_DOWNLOAD_FACTORY_HPP

#include <functional>
#include <string>
#include <boost/optional.hpp>
#include "download_factory.hpp"
#include "url.hpp"

namespace spider {

    class FileDownloadFactory : public virtual DownloadFactory {
        std::string m_downloadDirectory;
        uintmax_t m_minSize;

    public:
        FileDownloadFactory(std::string const& downloadDirectory, uintmax_t minSize);

        std::function<void(void)> create(
            Url const& url, 
            boost::optional<Url> referrer) const;
    };

}

#endif  // SPIDER_FILE_DOWNLOAD_FACTORY_HPP
