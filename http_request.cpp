#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>
#include <boost/asio.hpp>
#include "http_request.hpp"
#include "url.hpp"

namespace spider {

using namespace std;
using namespace boost;
using namespace boost::algorithm;
using namespace boost::asio;
using namespace boost::asio::ip;

string str(RequestMethod method) {
    switch (method) {
        case GET:
        default:
            return "GET";
        case POST:
            return "POST";
        case PUT:
            return "PUT";
        case DELETE:
            return "DELETE";
    }
}

string HttpRequest::getNewline() {
    return "\x0D\x0A";
}

HttpRequest::HttpRequest(RequestMethod method, Url const& url) 
    : m_method(method), m_url(url) {
}

void HttpRequest::setHeader(string const& name, string const& value) {
    if (trim_copy(name) == "") {
        throw invalid_argument("A header name cannot be blank.");
    }
    m_headers[name] = value;
}

void HttpRequest::getResponse() const {
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
