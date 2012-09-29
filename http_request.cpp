#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>
#include <boost/asio.hpp>
#include "header.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "url.hpp"

namespace {

std::string const& str(spider::RequestMethod method) {
    using std::string;

    static const string getString = "GET";
    static const string postString = "POST";
    static const string putString = "PUT";
    static const string deleteString = "DELETE";
    static const string headString = "HEAD";
    static const string traceString = "TRACE";
    static const string connectString = "CONNECT";

    switch (method) {
        case spider::GET:
        default:
            return getString;
        case spider::POST:
            return postString;
        case spider::PUT:
            return putString;
        case spider::DELETE:
            return deleteString;
        case spider::HEAD:
            return headString;
        case spider::TRACE:
            return traceString;
        case spider::CONNECT:
            return connectString;
    }
}

}

namespace spider {

std::string const& HttpRequest::getNewline() {
     static const std::string newline = "\r\n";
     return newline;
}

HttpRequest::HttpRequest(RequestMethod method, Url const& url)
    : m_method(method), m_url(url) {
}

HeaderCollection & HttpRequest::getHeaders() {
    return m_headers;
}

HttpRequest::response_ptr HttpRequest::getResponse() const {
    using std::istream;
    using std::ostream_iterator;
    using std::string;
    using std::stringstream;
    using boost::asio::ip::tcp;
    using boost::shared_ptr;

    string const& host = m_url.getHost();
    string const& scheme = m_url.getScheme();

    shared_ptr<tcp::iostream> tcpStream(new tcp::iostream());
    tcpStream->connect(host, scheme);

    shared_ptr<istream> stream;
    if (*tcpStream) {
        *tcpStream << str(m_method) << " " << m_url.getPath();
        string const& query = m_url.getQuery();
        if (query.size() > 0) {
            *tcpStream << "?" << query;
        }
        *tcpStream << " HTTP/1.0" << HttpRequest::getNewline();
        ostream_iterator<Header> destination(*tcpStream, HttpRequest::getNewline().c_str());
        copy(m_headers.begin(), m_headers.end(), destination);
        *tcpStream << HttpRequest::getNewline();
        stream = tcpStream;
    } else {
        shared_ptr<stringstream> stringStream(
            new stringstream("HTTP/1.1 500 Failed to connect"));
        stream = shared_ptr<istream>(stringStream);
    }

    response_ptr response(new HttpResponse(stream));
    return response;
}

}
