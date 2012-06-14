#include <algorithm>
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
        using std::istream_iterator;
        using std::noskipws;
        using std::ostream_iterator;
        using std::string;
        using boost::shared_ptr;
        
        HttpRequest request(GET, url);
        HttpRequest::response_ptr response = request.getResponse();
        
        int status = response->getStatusCode();
        if (status != 200) {
            // TODO: throw an exception
        }
        
        istream_iterator<char> begin(response->getContent() >> noskipws);
        istream_iterator<char> end;
        string content;
        copy(begin, end, back_inserter(content));
        return content;
    }

}
