#ifndef SPIDER_PAGE_DOWNLOADER_HPP
#define SPIDER_PAGE_DOWNLOADER_HPP

#include <string>
#include "url.hpp"

namespace spider {

class PageDownloader {
public:
    std::string download(Url const& url) const;
};

}

#endif
