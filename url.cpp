#include <exception>
#include <sstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include "url.hpp"

namespace spider {

Url::Url(
    std::string const& host,
    int port,
    std::string const& path,
    std::string const& query,
    std::string const& fragment,
    std::string const& scheme,
    std::string const& userInfo)
    : m_scheme(boost::to_lower_copy(scheme)),
    m_host(boost::to_lower_copy(host)),
    m_port(port),
    m_path(path),
    m_query(query),
    m_fragment(fragment),
    m_userInfo(userInfo) {
}

std::string const& Url::getDefaultScheme() {
    static const std::string scheme = "http";
    return scheme;
}

int Url::getDefaultPort() {
    return 0;
}

Url Url::parse(std::string const& urlString) {
    using std::istringstream;
    using std::string;
    using boost::replace_all;
    using boost::regex;
    using boost::regex_match;
    using boost::smatch;

    const static string urlFormat = "^((?<scheme>[a-zA-Z][a-zA-Z0-9+.-]*)://)?((?<userinfo>[^@]*)@)?(?<host>[a-zA-Z0-9.-]+)(:(?<port>[\\d]{1,5}))?(?<path>[/\\\\][^?#]*)?(\\?(?<query>[^#]*))?(#(?<fragment>.*))?$";
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
    string userInfo = matches["userinfo"];
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
    string fragment = matches["fragment"];
    // use '/' as default if there is a query but no path
    if (path == "" && (query != "" || fragment != "")) {
        path = "/";
    } else {
        replace_all(path, "\\", "/");
        replace_all(path, "//", "/");
    }
    return Url(host, port, path, query, fragment, scheme, userInfo);
}

std::ostream & operator<<(std::ostream & stream, Url const& url) {
    stream << url.getScheme() << "://";
    if (url.getUserInfo() != "") {
        stream << url.getUserInfo() << "@";
    }
    stream << url.getHost();
    if (url.getPort() != Url::getDefaultPort()) {
        stream << ":" << url.getPort();
    }
    if (url.getPath() != "") {
        stream << url.getPath();
    }
    if (url.getQuery() != "") {
        stream << "?" << url.getQuery();
    }
    if (url.getFragment() != "") {
        stream << "#" << url.getFragment();
    }
    return stream;
}

BadUrlException::BadUrlException(std::string const& url) throw() {
    using std::ostringstream;

    ostringstream builder;
    builder << "The given string was not a valid URL: " << url;
    m_what = builder.str();
}

BadUrlException::~BadUrlException() throw() {
}

char const* BadUrlException::what() const throw() {
    return m_what.c_str();
}

}
