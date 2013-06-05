#include <algorithm>
#include <functional>
#include <ios>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "downloader.hpp"
#include "download_manager.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "page_downloader.hpp"
#include "url.hpp"
#include "url_finder.hpp"

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
    DownloadManager & manager,
    UrlFinder const& finder) const {
    using std::string;
    using std::vector;

    HeaderCollection const& headers = response.getHeaders();
    if (!headers.hasHeader("Location")) {
        return;
    }
    string urlString = headers.getHeader("Location").getValue(0);
    try {
        Url const& referrer = getReferrer();
        vector<Url> redirectUrls { Url::parse(urlString) };
        manager.download(
            referrer, 
            redirectUrls.begin(), redirectUrls.end());
    } catch (...) {
    }
}

spider::PageDownloader::PageDownloader(
    Url const& url,
    Url const& referrer)
    : Downloader(url, referrer) {
}

void spider::PageDownloader::download(
    DownloadManager & manager,
    UrlFinder const& finder) const {
    using std::back_inserter;
    using std::string;
    using std::vector;

    try {
        Url const& url = getUrl();
        std::cerr << "Downloading page: " << url << std::endl;
    
        HttpResponse response = getResponse();

        int statusCode = response.getStatusCode();
        if (statusCode >= 300 && statusCode < 400) {
            handleRedirect(response, manager, finder);
            return;
        }
        
        string content = getContent(response);
        
        vector<Url> urls;
        finder.getUrls(url, content, back_inserter(urls));
        
        manager.download(url, urls.begin(), urls.end());

    } catch (ConnectionException const& exception) {
        std::cerr << exception.what() << std::endl;
    }
}
