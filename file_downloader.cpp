#include <algorithm>
#include <fstream>
#include <ios>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <boost/optional.hpp>
#include <boost/regex.hpp>
#include "downloader.hpp"
#include "file_downloader.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "path.hpp"
#include "url.hpp"

namespace {

    std::string encode(std::string const& value) {
        using std::string;
        using boost::regex;
        using boost::regex_replace;

        static regex search("[\\/:*?\"'<>|.,]");
        string result = regex_replace(value, search, "_");
        return result;
    }

    std::string createFileName(spider::Url const& url) {
        using std::ostringstream;
        using std::string;
        using spider::Path;

        string directory = Path::getDirectory(url.getPath());
        string fileName = Path::getFileName(url.getPath());

        ostringstream builder;
        builder << encode(url.getHost()) << encode(directory) << fileName;
        return builder.str();
    }

}

bool spider::FileDownloader::isLargeEnough() const {
    using std::istringstream;
    using std::string;

    if (m_minSize == 0) {
        return true;
    }

    try {
        Url const& url = getUrl();
        HttpRequest request(RequestMethod::HEAD, url);
        addReferrerHeader(request);
        addUserAgentHeader(request);
        addAcceptHeader(request);
        addHostHeader(request);
        addConnectionHeader(request);
        HttpResponse response = request.getResponse();

        if (response.getStatusCode() != 200) {
            return false;
        }

        HeaderCollection const& headers = response.getHeaders();
        if (headers.hasHeader("Content-Length")) {
            Header const& header = headers.getHeader("Content-Length");
            string contentLengthString = header.getValue(0);
            istringstream source(contentLengthString);
            uintmax_t contentLength = 0;
            if (source >> contentLength && contentLength < m_minSize) {
                return false;
            }
        }
        return true;
    }
    catch (ConnectionException const& exception) {
        std::cerr << exception.what() << std::endl;
        return false;
    }
}

void spider::FileDownloader::removeSmallFile(std::string const& path) const {
    if (m_minSize == 0) {
        return;
    }
    if (Path::size(path) < m_minSize) {
        Path::remove(path);
    }
}

spider::FileDownloader::FileDownloader(
    Url const& url, 
    boost::optional<Url> referrer,
    std::string const& downloadDirectory,
    uintmax_t minSize)
    : Downloader(url, referrer), m_downloadDirectory(downloadDirectory), m_minSize(minSize) {
}

void spider::FileDownloader::download() const {
    using std::copy;
    using std::ios;
    using std::istream;
    using std::istream_iterator;
    using std::noskipws;
    using std::ofstream;
    using std::ostream_iterator;
    using std::string;

    Url const& url = getUrl();
    if (url.getScheme() == "https") {
        return;
    }
        
    string fileName = createFileName(url);
    string path = m_downloadDirectory + '/' + fileName;
    if (Path::exists(path)) {
        return;
    }

    if (!isLargeEnough()) {
        return;
    }

    try {
        HttpRequest request(RequestMethod::GET, url);
        addReferrerHeader(request);
        addUserAgentHeader(request);
        addAcceptHeader(request);
        addHostHeader(request);
        addConnectionHeader(request);
        HttpResponse response = request.getResponse();

        if (response.getStatusCode() != 200) {
            return;
        }

        istream & stream = response.getContent();
        stream >> noskipws;
        if (!stream) {
            return;
        }

        std::cerr << "Downloading file: " << getUrl() << std::endl;
        
        istream_iterator<unsigned char> begin(stream);
        istream_iterator<unsigned char> end;
        
        ofstream file(path.c_str(), ios::out | ios::binary);
        ostream_iterator<unsigned char> destination(file);
        copy(begin, end, destination);

        removeSmallFile(path);

    } catch (ConnectionException const& exception) {
        std::cerr << exception.what() << std::endl;
    }
}
