#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include "http_response.hpp"

namespace spider {
    HttpResponse::HttpResponse(boost::shared_ptr<boost::asio::ip::tcp::iostream> stream)
        : m_stream(stream) {
    }
    
    bool getLine(std::istream & stream, std::string & line) {
        using std::getline;
        using boost::algorithm::is_any_of;
        using boost::algorithm::trim_right_if;
            
        getline(stream, line);
        trim_right_if(line, is_any_of("\r"));
        return stream && line.size() > 0;
    }

    void HttpResponse::getStatusCached() {
        if (!m_hasStatus) {
            using std::istringstream;
            using std::noskipws;
            using std::string;

            string statusLine;
            getLine(*m_stream, statusLine);
            istringstream reader(statusLine);
            string version;
            reader >> version; // discard HTTP version
            reader >> m_status;
            string message;
            reader >> noskipws >> message; // disregard OK
            // TODO: check for bad format
            m_hasStatus = true;
        }
    }

    int HttpResponse::getStatus() const {
        const_cast<HttpResponse&>(*this).getStatusCached();
        return m_status;
    }

    void HttpResponse::getHeadersCached() {
        if (!m_hasHeaders) {
            using std::find;
            using std::make_pair;
            using std::string;

            getStatusCached(); // cache the status code
            string line;
            while (getLine(*m_stream, line)) {
                string::iterator position = find(line.begin(), line.end(), ':');
                string name(line.begin(), position);
                string value(position + 2, line.end());
                // TODO: Check for bad format
                m_headers.insert(make_pair(name, value));
            }
            m_hasHeaders = true;
        }
    }
    
    std::istream & HttpResponse::getContent() {
        getHeadersCached();            
        return *m_stream;
    }
}
