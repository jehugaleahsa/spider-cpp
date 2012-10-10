#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "algorithm.hpp"
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

}

namespace spider {

    HttpResponse::HttpResponse(boost::shared_ptr<std::istream> stream)
        : m_stream(stream), m_hasStatus(), m_statusCode(), m_hasHeaders() {
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

    std::pair<std::string, std::string> parseHeaderPair(Line const& line) {
        using std::find;
        using std::pair;
        using std::string;
        using boost::algorithm::trim;

        string::const_iterator position = find(line.value.begin(), line.value.end(), ':');
        if (position == line.value.end()) {
            return pair<string, string>("", "");
        }
        string name(line.value.begin(), position);
        trim(name);
        string value(position + 1, line.value.end());
        trim(value);
        return pair<string, string>(name, value);
    }

    void HttpResponse::getHeadersCached() {
        using std::back_inserter;
        using std::equal_to;
        using std::for_each;
        using std::istream_iterator;
        using std::pair;
        using std::string;
        using std::remove_if;
        using std::transform;
        using std::vector;
        using boost::bind;

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
                    !bind(equal_to<string>(), bind(&Line::value, _1), string()));

                // split the lines at the first colon (:)
                vector<HeaderPair> headerPairs;
                transform(
                    lines.begin(), lines.end(),
                    back_inserter(headerPairs),
                    parseHeaderPair);

                // ignore any headers without a name
                vector<HeaderPair>::iterator pastHeaders = remove_if(
                    headerPairs.begin(), headerPairs.end(),
                    bind(equal_to<string>(),
                        bind(&HeaderPair::first, _1), string()));

                // add each header pair to the header collection
                for_each(
                    headerPairs.begin(), pastHeaders,
                    bind(&HeaderCollection::addHeader, &m_headers,
                        bind(&HeaderPair::first, _1),
                        bind(&HeaderPair::second, _1)));
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
