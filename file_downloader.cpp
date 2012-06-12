#include <algorithm>
#include <fstream>
#include <ios>
#include <iterator>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include "http_request.hpp"
#include "http_response.hpp"
#include "url.hpp"
#include "file_downloader.hpp"

namespace spider {
    
    FileDownloader::FileDownloader(std::string const& directoryPath)
        : m_directoryPath(directoryPath) {
    }
    
    namespace {
    
        bool isDuplicateDirectory(std::string const& directory) {
            return directory == "/";
        }
    
        void splitPath(std::string const& path, std::string & directory, std::string & fileName) {
            using std::find;
            using std::string;
            
            string::const_reverse_iterator rposition = find(path.rbegin(), path.rend(), '/');
            if (rposition == path.rend()) {
                directory = path;
                fileName = "";
            } else {
                string::const_iterator position = rposition.base();
                directory = string(path.begin(), position + 1);
                fileName = string(position + 1, path.end());
            }
        }
    
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

            string directory;
            string fileName;
            splitPath(url.getPath(), directory, fileName);
            
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
        
        HttpRequest request(GET, url);
        HttpResponse response = request.getResponse();
        
        istream_iterator<unsigned char> begin(response.getContent() >> noskipws);
        istream_iterator<unsigned char> end;
        
        string fileName = createFileName(url);
        string path = m_directoryPath + '/' + fileName;
        ofstream file(path.c_str(), ios::out | ios::binary);
        ostream_iterator<unsigned char> destination(file);
        
        copy(begin, end, destination);
    }
    
}
