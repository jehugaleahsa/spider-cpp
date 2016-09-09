#include <algorithm>
#include <functional>
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

std::string const& spider::HttpRequest::getNewline() {
     static const std::string newline = "\r\n";
     return newline;
}

spider::HttpRequest::HttpRequest(std::string const& method, Url const& url)
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

    *tcpStream << m_method << " " << m_url.getPath();

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
