#ifndef SPIDER_PAGE_DOWNLOADER_HPP
#define SPIDER_PAGE_DOWNLOADER_HPP

#include <string>
#include "downloader.hpp"
#include "url.hpp"

namespace spider {

class PageDownloader : public virtual Downloader {
public:
    std::string download(Url const& referrer, Url const& url) const;
};

}

#endif
