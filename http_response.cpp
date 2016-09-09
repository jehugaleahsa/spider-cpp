#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <boost/algorithm/string.hpp>
#include "algorithm.hpp"
#include "chunked_stream.hpp"
#include "header.hpp"
#include "http_response.hpp"

namespace {

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

    class HeaderParser : std::unary_function<Line const&, std::pair<std::string, std::string>> {
        std::string m_headerName;
    public:
        HeaderParser()
            : m_headerName() {
        }

        std::pair<std::string, std::string> operator() (Line const& line) {
            using std::find;
            using std::pair;
            using std::string;
            using boost::algorithm::trim;
            using boost::algorithm::trim_copy;

            string::const_iterator position = find(line.value.begin(), line.value.end(), ':');
            if (position == line.value.end()) {
                return pair<string, string>(m_headerName, trim_copy(line.value));
            } else {
                string name(line.value.begin(), position);
                trim(name);
                m_headerName = name;
                string value(position + 1, line.value.end());
                trim(value);
                return pair<string, string>(name, value);
            }
        }
    };

}

spider::HttpResponse::HttpResponse(std::shared_ptr<std::istream> stream)
    : m_stream(stream), m_hasStatus(), m_statusCode(), m_hasHeaders() {
}

spider::HttpResponse::HttpResponse(HttpResponse const& other)
    : 
    m_stream(other.m_stream), 
    m_hasStatus(other.m_hasStatus), 
    m_statusCode(other.m_statusCode), 
    m_hasHeaders(other.m_hasHeaders) {
}

void spider::HttpResponse::getStatusCached() const {
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
        }
    }
}

std::string spider::HttpResponse::getVersion() const {
    getStatusCached();
    return m_version;
}

int spider::HttpResponse::getStatusCode() const {
    getStatusCached();
    return m_statusCode;
}

std::string spider::HttpResponse::getStatusMessage() const {
    getStatusCached();
    return m_statusMessage;
}

void spider::HttpResponse::getHeadersCached() const {
    using std::back_inserter;
    using std::for_each;
    using std::istream_iterator;
    using std::pair;
    using std::string;
    using std::transform;
    using std::vector;

    typedef pair<string, string> HeaderPair;

    if (!m_hasHeaders) {
        getStatusCached();

        if (*m_stream) {
            istream_iterator<Line> begin(*m_stream);
            istream_iterator<Line> end;

            // grab each header line
            vector<Line> lines;
            copy_while(
                begin, end,
                back_inserter(lines),
                [](Line const& line) { 
                    return !line.value.empty(); 
                });

            // split the lines at the first colon (:)
            vector<HeaderPair> headerPairs;
            HeaderParser parser;
            transform(
                lines.begin(), lines.end(),
                back_inserter(headerPairs),
                parser);

            // add each header pair to the header collection
            for_each(
                headerPairs.begin(), headerPairs.end(),
                [&](HeaderPair const& pair) { 
                    m_headers.addHeader(pair.first, pair.second);
                });
            m_hasHeaders = true;
        }
        if (m_headers.hasHeader("Transfer-Encoding")) {
            Header const& header = m_headers.getHeader("Transfer-Encoding");
            if (header.getValue(0) == "chunked") {
                m_stream.reset(new ChunkedStream<char>(m_stream));
            }
        }
    }
}

spider::HeaderCollection const& spider::HttpResponse::getHeaders() const {
    getHeadersCached();
    return m_headers;
}

std::istream & spider::HttpResponse::getContent() {
    getHeadersCached();
    return *m_stream;
}
