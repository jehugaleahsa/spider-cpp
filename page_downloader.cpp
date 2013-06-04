#include <algorithm>
#include <functional>
#include <ios>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "categorizer.hpp"
#include "downloader.hpp"
#include "extractor.hpp"
#include "file_downloader.hpp"
#include "http_request.hpp"
#include "page_downloader.hpp"
#include "stripper.hpp"
#include "thread_pool.hpp"
#include "tracker.hpp"
#include "url.hpp"

spider::HttpResponse spider::PageDownloader::getResponse() const {
    Url const& url = getUrl();
    HttpRequest request(GET, url);
    addReferrerHeader(request);
    addUserAgentHeader(request);
    addAcceptHeader(request);
    addHostHeader(request);
    //addConnectionHeader(request);
    return request.getResponse();
}

std::string spider::PageDownloader::getContent(HttpResponse & response) const {
    using std::istream;
    using std::istream_iterator;
    using std::noskipws;
    using std::string;

    istream & stream = response.getContent();
    stream >> noskipws;
    istream_iterator<char> begin(stream);
    istream_iterator<char> end;
    return string(begin, end);
}
        
void spider::PageDownloader::handleRedirect(
    HttpResponse & response,
    std::string const& downloadDirectory,
    ThreadPool & pool,
    UrlTracker & tracker,
    Categorizer const& pageCategorizer,
    Categorizer const& mediaCategorizer,
    Stripper const& stripper,
    UrlExtractor const& baseExtractor,
    UrlExtractor const& extractor) const {
    using std::string;

    HeaderCollection const& headers = response.getHeaders();
    if (!headers.hasHeader("Location")) {
        return;
    }
    string urlString = headers.getHeader("Location").getValue(0);
    try {
        Url redirect = Url::parse(urlString);
        queuePageDownload(
            redirect, 
            true,
            downloadDirectory,
            pool,
            tracker,
            pageCategorizer,
            mediaCategorizer,
            stripper,
            baseExtractor,
            extractor);
    } catch (...) {
    }
}

spider::Url spider::PageDownloader::getBaseUrl(
    UrlExtractor const& baseExtractor,
    std::string const& content) const {
    using std::back_inserter;
    using std::vector;

    Url const& url = getUrl();
    vector<Url> baseAddresses;
    baseExtractor.getUrls(url, content, baseAddresses);
    if (baseAddresses.size() == 0) {
        return url;
    } else {
        return baseAddresses.back();
    }
}

void spider::PageDownloader::queuePageDownloads(
    std::vector<Url>::const_iterator begin,
    std::vector<Url>::const_iterator end,
    std::string const& downloadDirectory,
    ThreadPool & pool,
    UrlTracker & tracker,
    Categorizer const& pageCategorizer,
    Categorizer const& mediaCategorizer,
    Stripper const& stripper,
    UrlExtractor const& baseExtractor,
    UrlExtractor const& extractor) const {
    using std::for_each;
    using std::vector;

    for_each(begin, end, [&](Url const& url) {
        queuePageDownload(
            url,
            false,
            downloadDirectory,
            pool,
            tracker,
            pageCategorizer,
            mediaCategorizer,
            stripper,
            baseExtractor,
            extractor);
    });
}

void spider::PageDownloader::queuePageDownload(
    Url const& url, 
    bool reuseReferrer,
    std::string const& downloadDirectory,
    ThreadPool & pool,
    UrlTracker & tracker,
    Categorizer const& pageCategorizer,
    Categorizer const& mediaCategorizer,
    Stripper const& stripper,
    UrlExtractor const& baseExtractor,
    UrlExtractor const& extractor) const {
    using std::bind;
    using std::shared_ptr;

    if (tracker.addUrl(url)) {
        Url referrer = reuseReferrer ? getReferrer() : getUrl();
        pool.addTask([&, url, referrer]() { 
            PageDownloader downloader(url, referrer);
            downloader.download(
                downloadDirectory,
                pool,
                tracker,
                pageCategorizer,
                mediaCategorizer,
                stripper,
                baseExtractor,
                extractor); 
        });
    }
}

void spider::PageDownloader::queueFileDownloads(
    std::vector<Url>::const_iterator begin,
    std::vector<Url>::const_iterator end,
    ThreadPool & pool,
    UrlTracker & tracker,
    std::string const& downloadDirectory) const {
    using std::for_each;
    using std::vector;
    
    for_each(begin, end, [&](Url const& url) {
        queueFileDownload(url, pool, tracker, downloadDirectory);
    });
}

void spider::PageDownloader::queueFileDownload(
    Url const& url, 
    ThreadPool & pool,
    UrlTracker & tracker,
    std::string const& downloadDirectory) const {
    using std::shared_ptr;
    
    if (tracker.addUrl(url)) {
        Url referrer = getUrl();
        pool.addTask([&, url, referrer]() {
            FileDownloader downloader(url, referrer);
            downloader.download(downloadDirectory);
        });
    }
}

spider::PageDownloader::PageDownloader(
    Url const& url,
    Url const& referrer)
    : Downloader(url, referrer) {
}

void spider::PageDownloader::download(
    std::string const& downloadDirectory,
    ThreadPool & pool,
    UrlTracker & tracker,
    Categorizer const& pageCategorizer,
    Categorizer const& mediaCategorizer,
    Stripper const& stripper,
    UrlExtractor const& baseExtractor,
    UrlExtractor const& extractor) const {
    using std::string;
    using std::vector;

    try {
        std::cerr << "Downloading page: " << getUrl() << std::endl;
    
        HttpResponse response = getResponse();

        int statusCode = response.getStatusCode();
        if (statusCode >= 300 && statusCode < 400) {
            handleRedirect(
                response,
                downloadDirectory,
                pool,
                tracker,
                pageCategorizer,
                mediaCategorizer,
                stripper,
                baseExtractor,
                extractor);
            return;
        }

        string content = getContent(response);
        content = stripper.strip(content);

        Url baseUrl = getBaseUrl(baseExtractor, content);
        vector<Url> urls;
        extractor.getUrls(baseUrl, content, urls);

        vector<Url>::iterator pageEnd = partition(
            urls.begin(), urls.end(), 
            [&](Url const& url) { return pageCategorizer.isDesired(url); });
        queuePageDownloads(
            urls.begin(), pageEnd,
            downloadDirectory,
            pool,
            tracker,
            pageCategorizer,
            mediaCategorizer,
            stripper,
            baseExtractor,
            extractor);

        vector<Url>::iterator mediaEnd = partition(
            urls.begin(), urls.end(), 
            [&](Url const& url) { return mediaCategorizer.isDesired(url); });
        queueFileDownloads(
            urls.begin(), mediaEnd, 
            pool, 
            tracker, 
            downloadDirectory);
    } catch (ConnectionException const& exception) {
        std::cerr << exception.what() << std::endl;
    }
}
