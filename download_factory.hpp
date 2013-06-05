#ifndef SPIDER_DOWNLOAD_FACTORY
#define SPIDER_DOWNLOAD_FACTORY

#include <functional>
#include "url.hpp"

namespace spider {

    class DownloadFactory {
    public:
        virtual ~DownloadFactory() {
        }

        virtual std::function<void(void)> create(
            Url const& url, 
            Url const& referrer) const = 0;
    };

}

#endif  // SPIDER_DOWNLOAD_FACTORY
