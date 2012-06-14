#include <algorithm>
#include <fstream>
#include <ios>
#include <iterator>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>
#include "http_request.hpp"
#include "http_response.hpp"
#include "path_utilities.hpp"
#include "url.hpp"
#include "file_downloader.hpp"

namespace spider {
    
    FileDownloader::FileDownloader(std::string const& directoryPath)
        : m_directoryPath(directoryPath) {
    }
    
    namespace {
    
        std::string encode(std::string const& value) {
            using std::string;
            using boost::regex;
            using boost::regex_replace;
            
            static regex search("[\\/:*?\"'<>|.,]");
            string result = regex_replace(value, search, "_");
            return result;
        }
    
        std::string createFileName(Url const& url) {
            using std::ostringstream;
            using std::string;

            string directory = getDirectory(url.getPath());
            string fileName = getFileName(url.getPath());
            
            ostringstream builder;
            builder << encode(url.getHost()) << encode(directory) << fileName;
            return builder.str();
        }  
    }
    
    void FileDownloader::download(Url const& url) const {
        using std::copy;
        using std::ios;
        using std::istream_iterator;
        using std::noskipws;
        using std::ofstream;
        using std::ostream_iterator;
        using std::string;
        using boost::shared_ptr;
        
        HttpRequest request(GET, url);
        HttpRequest::response_ptr response = request.getResponse();
        
        if (response->getStatusCode() != 200) {
            // TODO: throw an exception
        }
        
        istream_iterator<unsigned char> begin(response->getContent() >> noskipws);
        istream_iterator<unsigned char> end;
        
        string fileName = createFileName(url);
        string path = m_directoryPath + '/' + fileName;
        ofstream file(path.c_str(), ios::out | ios::binary);
        ostream_iterator<unsigned char> destination(file);
        
        copy(begin, end, destination);
    }
    
}
