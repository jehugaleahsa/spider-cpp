#include <algorithm>
#include <ios>
#include <iostream>
#include <iterator>
#include <vector>
#include <boost/bind.hpp>
#include "download_queue.hpp"
#include "extractor.hpp"
#include "http_request.hpp"
#include "page_downloader.hpp"
#include "stripper.hpp"
#include "url.hpp"
#include "spider.hpp"

namespace {

    spider::Url getBaseUrl(
        spider::UrlExtractor const& extractor,
        spider::Url const& url,
        std::string const& content) {
        using std::back_inserter;
        using std::vector;
        using spider::Url;
        using spider::UrlExtractor;

        vector<Url> baseAddresses;
        Url baseUrl(
            url.getScheme(),
            url.getHost(),
            url.getPort(),
            url.getPath(),
            "");
        extractor.getUrls(baseUrl, content, back_inserter(baseAddresses));
        if (baseAddresses.size() == 0) {
            return baseUrl;
        } else {
            return baseAddresses.back();
        }
    }

    void extractUrls(
        spider::DownloadQueue & queue,
        spider::UrlExtractor const& extractor,
        spider::Url const& url,
        std::string const& content) {
        using std::back_inserter;
        using std::for_each;
        using std::vector;
        using spider::DownloadQueue;
        using spider::Url;

        vector<Url> links;
        extractor.getUrls(url, content, back_inserter(links));
        for_each(
            links.begin(), links.end(),
            bind(&DownloadQueue::addUrl, &queue, _1));
    }

}

namespace spider {

void Spider::run(std::ostream & output, Url const& topUrl) const {
    using std::endl;
    using std::string;
    using spider::DownloadQueue;
    using spider::PageDownloader;
    using spider::Stripper;
    using spider::Url;
    using spider::UrlExtractor;

    PageDownloader pageDownloader;
    Stripper stripper("script");
    UrlExtractor baseExtractor("base", "href");
    UrlExtractor anchorExtractor("a", "href");
    UrlExtractor imageExtractor("img", "src");

    DownloadQueue queue;
    queue.addUrl(topUrl);

    while (queue.hasMore()) {
        Url url = queue.getNextUrl();
        output << url << endl;

        string original = pageDownloader.download(url);
        string stripped = stripper.strip(original);

        Url baseUrl = getBaseUrl(baseExtractor, url, stripped);
        extractUrls(queue, anchorExtractor, baseUrl, stripped);
    }
}

}
