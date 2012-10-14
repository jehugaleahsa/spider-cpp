#include <algorithm>
#include <fstream>
#include <ios>
#include <iterator>
#include <sstream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>
#include "downloader.hpp"
#include "file_downloader.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "path_utilities.hpp"
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
        using spider::getDirectory;
        using spider::getFileName;

        string directory = getDirectory(url.getPath());
        string fileName = getFileName(url.getPath());

        ostringstream builder;
        builder << encode(url.getHost()) << encode(directory) << fileName;
        return builder.str();
    }

}

namespace spider {

    FileDownloader::FileDownloader(
        Counter & counter,
        Url const& url,
        boost::shared_ptr<Url> const referrer,
        std::string const& directoryPath
    )
        : Downloader(counter, url, referrer), m_directoryPath(directoryPath) {
    }

    void FileDownloader::download() {
        using std::copy;
        using std::ios;
        using std::istream;
        using std::istream_iterator;
        using std::noskipws;
        using std::ofstream;
        using std::ostream_iterator;
        using std::string;
        using boost::filesystem::exists;

        Url const& url = getUrl();
        if (url.getScheme() == "https") {
            return;
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
            if (!stream) {
                return;
            }

            string fileName = createFileName(url);
            string path = m_directoryPath + '/' + fileName;
            if (exists(path)) {
                return;
            }
            
            std::cerr << "Downloading file: " << getUrl() << std::endl;
            
            istream_iterator<unsigned char> begin(stream);
            istream_iterator<unsigned char> end;
            
            ofstream file(path.c_str(), ios::out | ios::binary);
            ostream_iterator<unsigned char> destination(file);
            copy(begin, end, destination);
        } catch (ConnectionException const& exception) {
            std::cerr << exception.what() << std::endl;
        }
    }

}
