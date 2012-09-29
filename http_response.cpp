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
#include "header.hpp"
#include "http_response.hpp"

namespace spider {
    HttpResponse::HttpResponse(boost::shared_ptr<std::istream> stream)
        : m_stream(stream), m_hasStatus(), m_statusCode(), m_hasHeaders() {
    }

    struct Line {
        std::string value;
    };

    std::istream & operator >>(std::istream & input, Line & line) {
        using std::getline;
        using boost::algorithm::is_any_of;
        using boost::algorithm::trim_right_if;

        if (input) {
            getline(input, line.value);
            trim_right_if(line.value, is_any_of("\r\n"));
        }
        return input;
    }

    void HttpResponse::getStatusCached() {
        using std::getline;
        using std::istringstream;
        using boost::algorithm::trim;

        if (!m_hasStatus) {
            Line line;
            if (*m_stream >> line) {
                istringstream reader(line.value);
                reader >> m_version;
                reader >> m_statusCode;
                getline(reader, m_statusMessage);
                trim(m_statusMessage);
                // TODO: check for bad format
                m_hasStatus = true;
            } else {
                m_version = "HTTP/1.1";
                m_statusCode = 500;
                m_statusMessage = "Failed to connect";
            }
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

    bool makeHeaderPair(Line const& line, std::string & name, std::string & value) {
        using std::find;
        using std::string;
        using boost::algorithm::trim;

        string::const_iterator position = find(line.value.begin(), line.value.end(), ':');
        if (position == line.value.end()) {
            name = "";
            value = "";
            return false;
        }
        name.assign(line.value.begin(), position);
        trim(name);
        value.assign(position + 1, line.value.end());
        trim(value);
        return true;
    }

    inline bool isEmpty(Line const& line) {
        return line.value.empty();
    }

    class HeaderAdded : public std::unary_function<void, std::string> {
        HeaderCollection & m_headers;

    public:
        HeaderAdded(HeaderCollection & headers) : m_headers(headers) {
        }

        void operator ()(Line const& line) {
            using std::pair;
            using std::string;

            string name;
            string value;
            if (makeHeaderPair(line, name, value)) {
                m_headers.addHeader(name, value);
            }
        }
    };

    void HttpResponse::getHeadersCached() {
        using std::istream_iterator;
        using std::not1;
        using std::ptr_fun;
        using std::string;

        if (!m_hasHeaders) {
            getStatusCached(); // cache the status code
            if (*m_stream) {
                istream_iterator<Line> begin(*m_stream);
                istream_iterator<Line> end;
                for_each_while(
                    begin, end,
                    not1(ptr_fun(isEmpty)),
                    HeaderAdded(m_headers));
                m_hasHeaders = true;
            }
        }
    }

    HeaderCollection const& HttpResponse::getHeaders() const {
        return m_headers;
    }

    std::istream & HttpResponse::getContent() {
        getHeadersCached();
        return *m_stream;
    }
}
