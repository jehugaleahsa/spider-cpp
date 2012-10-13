#include <algorithm>
#include <ios>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "categorizer.hpp"
#include "counter.hpp"
#include "downloader.hpp"
#include "extractor.hpp"
#include "file_downloader.hpp"
#include "http_request.hpp"
#include "page_downloader.hpp"
#include "stripper.hpp"
#include "thread_pool.hpp"
#include "tracker.hpp"
#include "url.hpp"

namespace spider {

    std::string PageDownloader::getContent() const {
        using std::istream;
        using std::istream_iterator;
        using std::noskipws;
        using std::string;

        Url const& url = getUrl();
        if (url.getScheme() == "https") {
            return "";
        }

        try {
            HttpRequest request(GET, url);
            addReferrerHeader(request);
            addUserAgentHeader(request);
            addAcceptHeader(request);
            addHostHeader(request);
            addConnectionHeader(request);
            HttpRequest::response_ptr response = request.getResponse();
            istream & stream = response->getContent();
            stream >> noskipws;
            istream_iterator<char> begin(stream);
            istream_iterator<char> end;
            return string(begin, end);
        } catch (ConnectionException const& exception) {
            std::cerr << exception.what() << std::endl;
            return "";
        }
    }

    Url PageDownloader::getBaseUrl(std::string const& content) const {
        using std::back_inserter;
        using std::vector;

        Url const& url = getUrl();
        vector<Url> baseAddresses;
        m_baseExtractor.getUrls(url, content, back_inserter(baseAddresses));
        if (baseAddresses.size() == 0) {
            return url;
        } else {
            return baseAddresses.back();
        }
    }

    void PageDownloader::queuePageDownloads(
        std::vector<Url>::const_iterator begin,
        std::vector<Url>::const_iterator end) {
        using std::for_each;
        using std::vector;
        using boost::bind;

        for_each(begin, end, bind(&PageDownloader::queuePageDownload, this, _1));
    }
    
    void PageDownloader::queuePageDownload(Url const& url) {
        using boost::bind;
        using boost::shared_ptr;
    
        if (m_tracker.addUrl(url)) {
            Counter & counter = getCounter();
            Url const& referrer = getUrl();
            shared_ptr<Downloader> downloader(new PageDownloader(
                counter,
                url,
                referrer,
                m_pool,
                m_tracker,
                m_pageCategorizer,
                m_mediaCategorizer,
                m_stripper,
                m_baseExtractor,
                m_anchorExtractor,
                m_imageExtractor
            ));
            m_pool.addTask(bind(&Downloader::download, downloader));
        }
    }

    void PageDownloader::queueFileDownloads(
        std::vector<Url>::const_iterator begin,
        std::vector<Url>::const_iterator end) {
        using std::for_each;
        using std::vector;
        using boost::bind;
        
        for_each(begin, end, bind(&PageDownloader::queueFileDownload, this, _1));
    }
    
    void PageDownloader::queueFileDownload(Url const& url) {
        using boost::bind;
        using boost::shared_ptr;
        
        if (m_tracker.addUrl(url)) {
            Counter & counter = getCounter();
            Url const& referrer = getUrl();
            shared_ptr<Downloader> downloadable(new FileDownloader(
                counter,
                url,
                referrer,
                "/home/travis/temp/"  // TODO - make this path configurable
            ));
            m_pool.addTask(bind(&Downloader::download, downloadable));
        }
    }

    PageDownloader::PageDownloader(
        Counter & counter,
        Url const& url,
        Url const& referrer,
        ThreadPool & pool,
        UrlTracker & tracker,
        Categorizer const& pageCategorizer,
        Categorizer const& mediaCategorizer,
        Stripper const& stripper,
        UrlExtractor const& baseExtractor,
        UrlExtractor const& anchorExtractor,
        UrlExtractor const& imageExtractor)
        :
        Downloader(counter, url, referrer),
        m_pool(pool),
        m_tracker(tracker),
        m_pageCategorizer(pageCategorizer),
        m_mediaCategorizer(mediaCategorizer),
        m_stripper(stripper),
        m_baseExtractor(baseExtractor),
        m_anchorExtractor(anchorExtractor),
        m_imageExtractor(imageExtractor) {
    }

    void PageDownloader::download() {
        using std::partition;
        using std::string;
        using std::vector;
        using boost::bind;

        std::cerr << "Downloading page: " << getUrl() << std::endl;

        string original = getContent();
        string stripped = m_stripper.strip(original);

        Url baseUrl = getBaseUrl(stripped);
        vector<Url> urls;
        m_anchorExtractor.getUrls(baseUrl, stripped, back_inserter(urls));
        m_imageExtractor.getUrls(baseUrl, stripped, back_inserter(urls));

        vector<Url>::iterator end1 = partition(urls.begin(), urls.end(),
            bind(&Categorizer::isDesired, &m_pageCategorizer, _1));
        queuePageDownloads(urls.begin(), end1);

        vector<Url>::iterator end2 = partition(end1, urls.end(),
            bind(&Categorizer::isDesired, &m_mediaCategorizer, _1));
        queueFileDownloads(end1, end2);
    }

}
