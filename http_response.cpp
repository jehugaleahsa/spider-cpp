#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include "http_response.hpp"

namespace spider {
    HttpResponse::HttpResponse(
        boost::shared_ptr<boost::asio::io_service> service,
        boost::shared_ptr<boost::asio::ip::tcp::socket> socket)
        : m_service(service), m_socket(socket), m_buffer(new boost::asio::streambuf) {
        }

    bool HttpResponse::readLine(std::string & line) {
        using std::getline;
        using std::istream;
        using std::string;
        using boost::system::error_code;
        using boost::system::system_error;
        using boost::asio::error::eof;
        using boost::asio::read_until;

        // NOTE: There may be lines in the buffer from previous runs.
        // We need to first try grabbing a line, then check for stream errors.
        // If the stream ran out of characters, we need to read more from
        // the socket. From there, we can grab the next line.
        istream reader(m_buffer.get());
        getline(reader, line);
        if (reader.eof()) {
            reader.clear();
            error_code error;
            read_until(*m_socket, *m_buffer, "\r\n", error);
            if (error) {
                if (error == eof) {
                    return false;
                } else {
                    throw system_error(error);
                }
            }
            string remaining;
            getline(reader, remaining);
            line += remaining;
        }
        if (line.size() > 0) {
            line.erase(--line.end()); // chop off trailing \r
        }
        return true;
    }

    void HttpResponse::getStatusCached() {
        if (!m_hasStatus) {
            using std::istringstream;
            using std::noskipws;
            using std::string;

            string statusString;
            readLine(statusString);
            istringstream reader(statusString);
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
            bool hasMore = readLine(line);
            while (hasMore && line.size() > 0) {
                string::iterator position = find(line.begin(), line.end(), ':');
                string name(line.begin(), position);
                string value(position + 2, line.end());
                // TODO: Check for bad format
                m_headers.insert(make_pair(name, value));
                hasMore = readLine(line);
            }
            m_hasHeaders = true;
        }
    }
}
