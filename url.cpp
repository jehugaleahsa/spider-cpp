#include <exception>
#include <sstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include "url.hpp"

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

Url Url::parse(std::string const& urlString) {
    using std::istringstream;
    using std::string;
    using boost::replace_all;
    using boost::regex;
    using boost::regex_match;
    using boost::smatch;

    const static string urlFormat = "((?<scheme>[a-zA-Z][a-zA-Z0-9+.-]*)://)?(?<host>[a-zA-Z0-9.-]+)(:(?<port>[\\d]+))?(?<path>/[^?]*)?(\\?(?<query>.*))?";
    regex expression(urlFormat, regex::icase);
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
        istringstream portStream(portString);
        portStream >> port;
    }
    string path = matches["path"];
    string query = matches["query"];
    // use '/' as default if there is a query but no path
    if (path == "" && query != "") {
        path = "/";
    } else {
        replace_all(path, "//", "/");
    }
    return Url(scheme, host, port, path, query);
}

std::ostream & operator<<(std::ostream & stream, Url const& url) {
    stream << url.getScheme() << "://";
    stream << url.getHost();
    if (url.getPort() != Url::getDefaultPort()) {
        stream << ":" << url.getPort();
    }
    if (url.getPath().size() > 0) {
        stream << url.getPath();
    }
    if (url.getQuery().size() > 0) {
        stream << "?" << url.getQuery();
    }
    return stream;
}

BadUrlException::BadUrlException() throw() {
}

BadUrlException::BadUrlException(std::string const& url) throw()
    : m_url(url) {
}

BadUrlException::~BadUrlException() throw() {
}

char const* BadUrlException::what() const throw() {
    using std::string;
    using std::ostringstream;
    using std::endl;

    ostringstream builder;
    builder << "The given string was not a valid URL: " << m_url << endl;
    string result = builder.str();
    return result.c_str();
}

}
