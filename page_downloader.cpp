#include <algorithm>
#include <functional>
#include <ios>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include "categorizer.hpp"
#include "counter.hpp"
#include "downloader.hpp"
#include "extractor.hpp"
#include "file_downloader.hpp"
#include "http_request.hpp"
#include "page_downloader.hpp"
#include "scoped_counter.hpp"
#include "stripper.hpp"
#include "thread_pool.hpp"
#include "tracker.hpp"
#include "url.hpp"

spider::HttpRequest::response_ptr spider::PageDownloader::getResponse() const {
    Url const& url = getUrl();
    try {
        HttpRequest request(GET, url);
        addReferrerHeader(request);
        addUserAgentHeader(request);
        addAcceptHeader(request);
        addHostHeader(request);
        //addConnectionHeader(request);
        HttpRequest::response_ptr response = request.getResponse();
        return response;
    } catch (ConnectionException const& exception) {
        std::cerr << exception.what() << std::endl;
        return HttpRequest::response_ptr();
    }
}

std::string spider::PageDownloader::getContent(HttpRequest::response_ptr response) const {
    using std::istream;
    using std::istream_iterator;
    using std::noskipws;
    using std::string;

    istream & stream = response->getContent();
    stream >> noskipws;
    istream_iterator<char> begin(stream);
    istream_iterator<char> end;
    return string(begin, end);
}

spider::Url spider::PageDownloader::getBaseUrl(std::string const& content) const {
    using std::back_inserter;
    using std::vector;

    Url const& url = getUrl();
    vector<Url> baseAddresses;
    m_baseExtractor.getUrls(url, content, baseAddresses);
    if (baseAddresses.size() == 0) {
        return url;
    } else {
        return baseAddresses.back();
    }
}

void spider::PageDownloader::queuePageDownloads(
    std::vector<Url>::const_iterator begin,
    std::vector<Url>::const_iterator end) {
    using std::bind;
    using std::for_each;
    using std::placeholders::_1;
    using std::vector;

    for_each(begin, end, bind(&PageDownloader::queuePageDownload, this, _1, false));
}

void spider::PageDownloader::queuePageDownload(Url const& url, bool reuseReferrer) {
    using std::bind;
    using std::shared_ptr;

    if (m_tracker.addUrl(url)) {
        Counter & counter = getCounter();
        shared_ptr<Url> referrer;
        if (reuseReferrer) {
            referrer = getReferrer();
        } else { 
            referrer = shared_ptr<Url>(new Url(getUrl()));
        }
        shared_ptr<Downloader> downloader(new PageDownloader(
            counter,
            url,
            referrer,
            m_downloadDirectory,
            m_pool,
            m_tracker,
            m_pageCategorizer,
            m_mediaCategorizer,
            m_stripper,
            m_baseExtractor,
            m_extractor
        ));
        m_pool.addTask(bind(&Downloader::download, downloader));
    }
}

void spider::PageDownloader::queueFileDownloads(
    std::vector<Url>::const_iterator begin,
    std::vector<Url>::const_iterator end) {
    using std::bind;
    using std::for_each;
    using std::placeholders::_1;
    using std::vector;
    
    for_each(begin, end, bind(&PageDownloader::queueFileDownload, this, _1));
}

void spider::PageDownloader::queueFileDownload(Url const& url) {
    using std::bind;
    using std::shared_ptr;
    
    if (m_tracker.addUrl(url)) {
        Counter & counter = getCounter();
        shared_ptr<Url> referrer(new Url(getUrl()));
        shared_ptr<Downloader> downloadable(new FileDownloader(
            counter,
            url,
            referrer,
            m_downloadDirectory
        ));
        m_pool.addTask(bind(&Downloader::download, downloadable));
    }
}

spider::PageDownloader::PageDownloader(
    Counter & counter,
    Url const& url,
    std::shared_ptr<Url> const referrer,
    std::string const& downloadDirectory,
    ThreadPool & pool,
    UrlTracker & tracker,
    Categorizer const& pageCategorizer,
    Categorizer const& mediaCategorizer,
    Stripper const& stripper,
    UrlExtractor const& baseExtractor,
    UrlExtractor const& extractor)
    :
    Downloader(counter, url, referrer),
    m_downloadDirectory(downloadDirectory),
    m_pool(pool),
    m_tracker(tracker),
    m_pageCategorizer(pageCategorizer),
    m_mediaCategorizer(mediaCategorizer),
    m_stripper(stripper),
    m_baseExtractor(baseExtractor),
    m_extractor(extractor) {
}

void spider::PageDownloader::download() {
    using std::bind;
    using std::string;
    using std::placeholders::_1;
    using std::vector;

    std::cerr << "Downloading page: " << getUrl() << std::endl;

    HttpRequest::response_ptr response = getResponse();
    if (!response) {
        std::cerr << "There was not response to: " << getUrl() << std::endl;
        return;
    }

    string original = getContent(response);

    int statusCode = response->getStatusCode();
    if (statusCode >= 300 && statusCode < 400) {
        try {
            string urlString = original;
            if (urlString.size() == 0) {
                HeaderCollection const& headers = response->getHeaders();
                if (headers.hasHeader("Location")) {
                    urlString = headers.getHeader("Location").getValue(0);
                } else {
                    return;
                }
            }
            Url redirect = Url::parse(urlString);
            queuePageDownload(redirect, true);
        } catch (BadUrlException const& exception) {
            return;
        }
    }

    string stripped = m_stripper.strip(original);

    Url baseUrl = getBaseUrl(stripped);
    vector<Url> urls;
    m_extractor.getUrls(baseUrl, stripped, urls);

    vector<Url>::iterator end = partition(urls.begin(), urls.end(),
        bind(&Categorizer::isDesired, &m_pageCategorizer, _1));
    queuePageDownloads(urls.begin(), end);

    end = partition(urls.begin(), urls.end(),
        bind(&Categorizer::isDesired, &m_mediaCategorizer, _1));
    queueFileDownloads(urls.begin(), end);
}
