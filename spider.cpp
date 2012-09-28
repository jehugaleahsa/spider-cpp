#include <algorithm>
#include <ios>
#include <iostream>
#include <iterator>
#include <vector>
#include <boost/bind.hpp>
#include "categorizer.hpp"
#include "download_queue.hpp"
#include "extractor.hpp"
#include "file_downloader.hpp"
#include "page_downloader.hpp"
#include "spider.hpp"
#include "stripper.hpp"
#include "url.hpp"

namespace {

    void supportPageExtensions(spider::Categorizer & categorizer) {
        categorizer.supportExtension("");
        categorizer.supportExtension("htm");
        categorizer.supportExtension("html");
        categorizer.supportExtension("xhtml");
        categorizer.supportExtension("jsp");
        categorizer.supportExtension("php");
        categorizer.supportExtension("asp");
        categorizer.supportExtension("aspx");
        categorizer.supportExtension("cgi");
        categorizer.supportExtension("shtml");
        categorizer.supportExtension("cfm");
        categorizer.supportExtension("cfml");
    }

    void supportMediaExtensions(spider::Categorizer & categorizer) {
        categorizer.supportExtension("mpg");
        categorizer.supportExtension("mpeg");
        categorizer.supportExtension("mp4");
        categorizer.supportExtension("avi");
        categorizer.supportExtension("wmv");
        categorizer.supportExtension("mov");
        categorizer.supportExtension("rm");
        categorizer.supportExtension("png");
        categorizer.supportExtension("gif");
        categorizer.supportExtension("jpg");
        categorizer.supportExtension("tif");
        categorizer.supportExtension("bmp");
    }

    spider::Url getBaseUrl(
        spider::UrlExtractor const& extractor,
        spider::Url const& url,
        std::string const& content) {
        using std::back_inserter;
        using std::vector;
        using spider::Url;

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

}

namespace spider {

void Spider::run(std::ostream & output, Url const& topUrl) const {
    using std::back_inserter;
    using std::endl;
    using std::partition;
    using std::string;
    using std::vector;
    using boost::bind;

    Categorizer pageCategorizer;
    supportPageExtensions(pageCategorizer);

    Categorizer mediaCategorizer;
    supportMediaExtensions(mediaCategorizer);

    PageDownloader pageDownloader;
    FileDownloader fileDownloader("/home/travis/temp/");
    Stripper stripper("script");
    UrlExtractor baseExtractor("base", "href");
    UrlExtractor anchorExtractor("a", "href");
    UrlExtractor imageExtractor("img", "src");

    DownloadQueue queue;
    queue.addUrl(topUrl, topUrl, 0);

    while (queue.hasMore()) {
        UrlContext context = queue.getNextUrl();
        output << context.getUrl() << endl;

        if (pageCategorizer.isDesired(context.getUrl())) {
            string original = pageDownloader.download(
                context.getReferrer(), context.getUrl());
            string stripped = stripper.strip(original);

            Url baseUrl = getBaseUrl(baseExtractor, context.getUrl(), stripped);
            vector<Url> urls;
            anchorExtractor.getUrls(baseUrl, stripped, back_inserter(urls));
            imageExtractor.getUrls(baseUrl, stripped, back_inserter(urls));

            for_each(
                urls.begin(), urls.end(),
                bind(
                    &DownloadQueue::addUrl,
                    &queue,
                    context.getUrl(),
                    _1,
                    context.getDepth() + 1));
        } else if (mediaCategorizer.isDesired(context.getUrl())) {
            fileDownloader.download(context.getReferrer(), context.getUrl());
        }
    }
}

}
