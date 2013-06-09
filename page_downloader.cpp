#include <algorithm>
#include <functional>
#include <ios>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <boost/optional.hpp>
#include "downloader.hpp"
#include "download_manager.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "page_downloader.hpp"
#include "url.hpp"
#include "url_finder.hpp"

spider::HttpResponse spider::PageDownloader::getResponse() const {
    Url const& url = getUrl();
    HttpRequest request(RequestMethod::GET, url);
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
        
void spider::PageDownloader::handleRedirect(HttpResponse & response) const {
    using std::string;
    using std::vector;
    using boost::optional;

    HeaderCollection const& headers = response.getHeaders();
    if (!headers.hasHeader("Location")) {
        return;
    }
    string urlString = headers.getHeader("Location").getValue(0);
    optional<Url> referrer = getReferrer();
    vector<Url> redirectUrls { Url::parse(urlString) };
    m_manager.download(
       referrer, 
       redirectUrls.begin(), redirectUrls.end());
}

spider::PageDownloader::PageDownloader(
    Url const& url,
    boost::optional<Url> referrer,
    DownloadManager & manager,
    UrlFinder const & finder)
    : Downloader(url, referrer), m_manager(manager), m_finder(finder) {
}

void spider::PageDownloader::download() const {
    using std::back_inserter;
    using std::string;
    using std::vector;

    try {
        Url const& url = getUrl();
        std::cerr << "Downloading page: " << url << std::endl;
    
        HttpResponse response = getResponse();

        int statusCode = response.getStatusCode();
        if (statusCode >= 300 && statusCode < 400) {
            handleRedirect(response);
            return;
        }
        
        string content = getContent(response);
        
        vector<Url> urls;
        m_finder.getUrls(url, content, back_inserter(urls));
        
        m_manager.download(url, urls.begin(), urls.end());

    } catch (ConnectionException const& exception) {
        std::cerr << exception.what() << std::endl;
    }
}
