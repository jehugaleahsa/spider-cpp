#include <string>
#include <sstream>
#include <boost/regex.hpp>
#include "url.hpp"
#include "bad_url_exception.hpp"

namespace spider {

Url::Url(
    std::string const& scheme, 
    std::string const& host,
    int port,
    std::string const& path,
    std::string const& query)
    : m_scheme(scheme), 
    m_host(host), 
    m_port(port), 
    m_path(path), 
    m_query(query) {
}

std::string const& Url::getDefaultScheme() {
    static const std::string scheme = "http";
    return scheme;
}

int Url::getDefaultPort() {
    return 80;
}

std::string const& Url::getScheme() const {
    return m_scheme;
}

std::string const& Url::getHost() const {
    return m_host;
}

int Url::getPort() const {
    return m_port;
}

std::string const& Url::getPath() const {
    return m_path;
}

std::string const& Url::getQuery() const {
    return m_query;
}

Url Url::parse(std::string const& urlString) {
    using std::string;
    using std::stringstream;
    using boost::regex;
    using boost::regex_match;
    using boost::smatch;

    const string urlFormat = "((?<scheme>[a-zA-Z][a-zA-Z0-9+.-]*)://)?(?<host>[a-zA-Z0-9.-]+)(:(?<port>[\\d]+))?(?<path>/[^?]*)?(\\?(?<query>.*))?";
    regex expression(urlFormat);
    smatch matches;
    bool found = regex_match(urlString, matches, expression);
    if (!found) {
        throw BadUrlException(urlString);
    }
    string scheme = matches["scheme"];
    if (scheme == "") {
        scheme = Url::getDefaultScheme();
    }
    string host = matches["host"];
    string portString = matches["port"];
    int port;
    if (portString == "") {
        port = Url::getDefaultPort();
    } else {
        stringstream portStream(portString);
        portStream >> port;
    }
    string path = matches["path"];
    string query = matches["query"];
    // use '/' as default if there is a query but no path
    if (path == "" && query != "") {
        path = "/";
    }
    return Url(scheme, host, port, path, query);
}

}
