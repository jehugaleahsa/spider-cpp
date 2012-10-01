#include <algorithm>
#include <ios>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include "downloader.hpp"
#include "http_request.hpp"
#include "page_downloader.hpp"
#include "url.hpp"

namespace spider {

    std::string PageDownloader::download(
        Url const& referrer,
        Url const& url) const {
        using std::istream;
        using std::istream_iterator;
        using std::noskipws;
        using std::string;

        if (url.getScheme() == "https") {
            return "";
        }

        try {
            HttpRequest request(GET, url);
            Downloader::addReferrerHeader(request, referrer);
            Downloader::addUserAgentHeader(request);
            Downloader::addAcceptHeader(request);
            Downloader::addHostHeader(request, url);
            Downloader::addConnectionHeader(request);
            HttpRequest::response_ptr response = request.getResponse();
            istream & stream = response->getContent();
            stream >> noskipws;
            istream_iterator<char> begin(stream);
            istream_iterator<char> end;
            string content(begin, end);
            return content;
        } catch (ConnectionException const& exception) {
            std::cerr << exception.what() << std::endl;
            return "";
        }
    }

}
