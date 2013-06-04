#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <unordered_map>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
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

std::string const& spider::HttpRequest::getNewline() {
     static const std::string newline = "\r\n";
     return newline;
}

spider::HttpRequest::HttpRequest(RequestMethod method, Url const& url)
    : m_method(method), m_url(url) {
}

spider::HeaderCollection & spider::HttpRequest::getHeaders() {
    return m_headers;
}

spider::HttpResponse spider::HttpRequest::getResponse() const {
    using std::ostream_iterator;
    using std::make_shared;
    using std::shared_ptr;
    using std::string;
    using boost::asio::ip::tcp;
    using boost::lexical_cast;

    shared_ptr<tcp::iostream> tcpStream = make_shared<tcp::iostream>();
    if (m_url.getPort() == Url::getDefaultPort()) {
        tcpStream->connect(m_url.getHost(), m_url.getScheme());
    } else {
        tcpStream->connect(
            m_url.getHost(), lexical_cast<string>(m_url.getPort()));
    }

    if (!*tcpStream) {
        throw ConnectionException(m_url);
    }

    *tcpStream << str(m_method) << " " << m_url.getPath();

    string const& query = m_url.getQuery();
    if (query != "") {
        *tcpStream << "?" << query;
    }
    *tcpStream << " HTTP/1.1" << HttpRequest::getNewline();

    ostream_iterator<Header> destination(
        *tcpStream, HttpRequest::getNewline().c_str());
    m_headers.getHeaders(destination);

    *tcpStream << HttpRequest::getNewline();
    tcpStream->flush();

    return HttpResponse(tcpStream);
}

spider::ConnectionException::ConnectionException(Url const& url) throw() {
    using std::ostringstream;

    ostringstream builder;
    builder << "Failed to connect to " << url;
    m_what = builder.str();
}

spider::ConnectionException::~ConnectionException() throw() {
}

char const* spider::ConnectionException::what() const throw() {
    return m_what.c_str();
}
