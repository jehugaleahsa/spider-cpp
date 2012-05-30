#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>
#include <boost/asio.hpp>
#include "http_request.hpp"
#include "url.hpp"

namespace spider {

std::string const& str(RequestMethod method) {
    using std::string;

    static const string getString = "GET";
    static const string postString = "POST";
    static const string putString = "PUT";
    static const string deleteString = "DELETE";

    switch (method) {
        case GET:
        default:
            return getString;
        case POST:
            return postString;
        case PUT:
            return putString;
        case DELETE:
            return deleteString;
    }
}

std::string const& HttpRequest::getNewline() {
     static const std::string newline = "\x0D\x0A";
     return newline;
}

HttpRequest::HttpRequest(RequestMethod method, Url const& url) 
    : m_method(method), m_url(url) {
}

void HttpRequest::setHeader(std::string const& name, std::string const& value) {
    using std::invalid_argument;
    using boost::algorithm::trim_copy;

    if (trim_copy(name) == "") {
        throw invalid_argument("A header name cannot be blank.");
    }
    m_headers[name] = value;
}

void HttpRequest::getResponse() const {
    using std::string;
    using std::ostream;
    using boost::unordered_map;
    using boost::asio::ip::tcp;
    using boost::asio::io_service;

    string const& host = m_url.getHost();
    string const& scheme = m_url.getScheme();
    tcp::resolver::query nameQuery(host, scheme);

    io_service service;
    tcp::resolver resolver(service);
    tcp::resolver::iterator iterator = resolver.resolve(nameQuery);

    tcp::socket socket(service);
    connect(socket, iterator);

    boost::asio::streambuf request;
    ostream requestStream(&request);
    requestStream << str(m_method) << " " << m_url.getPath();
    string const& query = m_url.getQuery();
    if (query != "") {
        requestStream << "?" << query;
    }
    requestStream << " HTTP/1.0" << HttpRequest::getNewline();
    typedef unordered_map<string, string>::const_iterator map_iter;
    for (map_iter iterator = m_headers.begin(); iterator != m_headers.end(); ++iterator) {
        requestStream << iterator->first << ": " << iterator->second << HttpRequest::getNewline();
    }
    requestStream << HttpRequest::getNewline();

    write(socket, request);
}

}
