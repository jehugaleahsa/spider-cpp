#include <iterator>
#include <sstream>
#include <string>
#include "http_request.hpp"
#include "http_response.hpp"
#include "page_downloader.hpp"
#include "url.hpp"

namespace spider {

    std::string PageDownloader::download(Url const& url) const {
        using std::ostringstream;
        using std::ostream_iterator;
        
        HttpRequest request(GET, url);
        HttpResponse response = request.getResponse();
        int status = response.getStatus();
        if (status != 200) {
            // TODO: throw an exception
        }
        ostringstream htmlBuilder;
        ostream_iterator<char> destination(htmlBuilder);
        while (response.getNextContentChunk<ostream_iterator<char>, char>(destination)) {
        }
        return htmlBuilder.str();
    }

}
