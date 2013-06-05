#ifndef SPIDER_DOWNLOAD_FACTORY
#define SPIDER_DOWNLOAD_FACTORY

#include <functional>
#include <boost/optional.hpp>
#include "url.hpp"

namespace spider {

    class DownloadFactory {
    public:
        virtual ~DownloadFactory() {
        }

        virtual std::function<void(void)> create(
            Url const& url, 
            boost::optional<Url> referrer) const = 0;
    };

}

#endif  // SPIDER_DOWNLOAD_FACTORY
