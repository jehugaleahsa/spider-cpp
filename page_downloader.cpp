#include <algorithm>
#include <ios>
#include <iostream>
#include <iterator>
#include <string>
#include <boost/shared_ptr.hpp>
#include "http_request.hpp"
#include "http_response.hpp"
#include "page_downloader.hpp"
#include "url.hpp"

namespace spider {

    std::string PageDownloader::download(Url const& url) const {
        using std::back_inserter;
        using std::copy;
        using std::istream;
        using std::istream_iterator;
        using std::noskipws;
        using std::ostream_iterator;
        using std::string;
        using boost::shared_ptr;

        HttpRequest request(GET, url);
        HttpRequest::response_ptr response = request.getResponse();
        istream & stream = response->getContent();
        stream >> noskipws;
        istream_iterator<char> begin(stream);
        istream_iterator<char> end;
        string content(begin, end);
        return content;
    }

}
