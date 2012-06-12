#include <functional>
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>
#include <boost/asio.hpp>
#include "http_request.hpp"
#include "http_response.hpp"
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
     static const std::string newline = "\r\n";
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

std::string createHeader(std::pair<std::string const, std::string> const& pair) {
    return pair.first + ": " + pair.second;
}

HttpResponse HttpRequest::getResponse() const {
    using std::ostream;
    using std::ostream_iterator;
    using std::string;
    using std::transform;
    using boost::asio::io_service;
    using boost::asio::ip::tcp;
    using boost::shared_ptr;
    using boost::unordered_map;
    
    string const& host = m_url.getHost();
    string const& scheme = m_url.getScheme();
    
    shared_ptr<tcp::iostream> stream(new tcp::iostream());
    stream->connect(host, scheme);
    
    // TODO - check if we couldn't connect
    
    *stream << str(m_method) << " " << m_url.getPath();
    string const& query = m_url.getQuery();
    if (query != "") {
        *stream << "?" << query;
    }
    *stream << " HTTP/1.0" << HttpRequest::getNewline();
    ostream_iterator<string> destination(*stream, HttpRequest::getNewline().c_str());
    transform(m_headers.begin(), m_headers.end(), destination, createHeader);
    *stream << HttpRequest::getNewline();

    HttpResponse response(stream);
    return response;
}

}
