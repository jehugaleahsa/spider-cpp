#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <boost/algorithm/string.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include "algorithm.hpp"
#include "http_response.hpp"

namespace spider {
    HttpResponse::HttpResponse(boost::shared_ptr<std::istream> stream)
        : m_stream(stream) {
    }
    
    struct Line {
        std::string value;
    };
    
    std::istream & operator >>(std::istream & input, Line & line) {
        using std::getline;
        using boost::algorithm::is_any_of;
        using boost::algorithm::trim_right_if;
            
        getline(input, line.value);
        trim_right_if(line.value, is_any_of("\r"));        
        return input;
    }

    void HttpResponse::getStatusCached() {
        if (!m_hasStatus) {
            using std::istringstream;
            using std::noskipws;
            using std::string;

            Line line;
            *m_stream >> line;
            istringstream reader(line.value);
            reader >> m_version;
            reader >> m_statusCode;
            reader >> noskipws >> m_statusMessage;
            // TODO: check for bad format
            m_hasStatus = true;
        }
    }
    
    std::string HttpResponse::getVersion() {
        getStatusCached();
        return m_version;
    }

    int HttpResponse::getStatusCode() {
        getStatusCached();
        return m_statusCode;
    }
    
    std::string HttpResponse::getStatusMessage() {
        getStatusCached();
        return m_statusMessage;
    }
    
    std::pair<std::string, std::string> makeHeaderPair(Line const& line) {
        using std::find;
        using std::string;
        using boost::algorithm::trim;
        
        string::const_iterator position = find(line.value.begin(), line.value.end(), ':');

        string name = string(line.value.begin(), position);
        trim(name);
        string value = string(position + 1, line.value.end());
        trim(value);
        
        // TODO: Check for bad format
        return make_pair(name, value);
    }
    
    inline bool isEmpty(Line const& line) {
        return line.value.empty();
    }

    void HttpResponse::getHeadersCached() {
        if (!m_hasHeaders) {
            using std::inserter;
            using std::istream_iterator;
            using std::not1;
            using std::ptr_fun;
            using std::string;

            getStatusCached(); // cache the status code
            string line;
            istream_iterator<Line> begin(*m_stream);
            istream_iterator<Line> end;
            transform_while(
                begin, end, 
                inserter(m_headers, m_headers.end()), 
                not1(ptr_fun(isEmpty)), 
                ptr_fun(makeHeaderPair));
            m_hasHeaders = true;
        }
    }
    
    std::istream & HttpResponse::getContent() {
        getHeadersCached();            
        return *m_stream;
    }
}
