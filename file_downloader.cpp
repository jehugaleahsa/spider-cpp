#include <algorithm>
#include <fstream>
#include <ios>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <boost/array.hpp>
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
            using std::copy;
            using std::distance;
            using std::find;
            using std::ostream_iterator;
            using std::ostringstream;
            using std::remove_if;
            using std::string;
            using std::vector;
            
            // divide the sub-directories and the file by looking for the slashes
            vector<string> directories;
            string::const_iterator position = path.begin();
            while (position != path.end()) {
                string::const_iterator nextPosition = find(position, path.end(), '/');
                if (nextPosition != path.end()) {
                    ++nextPosition;
                    string subDirectory(position, nextPosition);
                    directories.push_back(subDirectory);
                } else {
                    // store everything after the last slash as the file name
                    fileName = string(position, path.end());
                }
                position = nextPosition;
            }
            
            // remove duplicate slashes
            if (distance(directories.begin(), directories.end()) > 1) {
                vector<string>::iterator position = remove_if(++directories.begin(), directories.end(), isDuplicateDirectory);
                directories.erase(position, directories.end());
            }
            
            // combine the relative paths into the absolute directory path
            ostringstream pathBuilder;
            ostream_iterator<string> destination(pathBuilder);
            copy(directories.begin(), directories.end(), destination);
            directory = pathBuilder.str();
        }
    
        std::string encode(std::string const& value) {
            using std::back_inserter;
            using std::copy;
            using std::find_first_of;
            using std::string;
            using boost::array;
            
            array<int, 12> badValues = { '\\', '/', ':', '*', '?', '"', '\'', '<', '>', '|', '.', ',' };
            string result;
            string::const_iterator position = value.begin();
            while (position != value.end()) {
                string::const_iterator nextPosition = find_first_of(position, value.end(), badValues.begin(), badValues.end());
                copy(position, nextPosition, back_inserter(result));
                if (nextPosition != value.end()) {
                    result += '_';
                    ++nextPosition;
                }
                position = nextPosition;
            }
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
        using std::ios;
        using std::ofstream;
        using std::ostream_iterator;
        using std::string;
        
        string fileName = createFileName(url);
        string path = m_directoryPath + '/' + fileName;
        ofstream file(path.c_str(), ios::out | ios::binary);
        ostream_iterator<unsigned char> destination(file);
        
        HttpRequest request(GET, url);
        HttpResponse response = request.getResponse();
        while (response.getNextContentChunk<ostream_iterator<unsigned char>, unsigned char>(destination)) {
        }
    }
    
}
