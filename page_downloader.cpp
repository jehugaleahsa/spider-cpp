#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include "http_request.hpp"
#include "http_response.hpp"
#include "page_downloader.hpp"
#include "url.hpp"

namespace spider {

    std::string PageDownloader::download(Url const& url) const {
        using std::copy;
        using std::istream_iterator;
        using std::ostringstream;
        using std::ostream_iterator;
        
        HttpRequest request(GET, url);
        HttpResponse response = request.getResponse();
        int status = response.getStatus();
        if (status != 200) {
            // TODO: throw an exception
        }
        istream_iterator<char> begin(response.getContent());
        istream_iterator<char> end;
        ostringstream htmlBuilder;
        ostream_iterator<char> destination(htmlBuilder);
        copy(begin, end, destination);
        return htmlBuilder.str();
    }

}
