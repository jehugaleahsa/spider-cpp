#include <string>
#include <sstream>
#include <boost/regex.hpp>
#include "url.hpp"

using namespace std;
using namespace boost;
using namespace spider;

namespace spider {

Url::Url(
    string const& scheme, 
    string const& host,
    int port,
    string const& path,
    string const& query)
    : m_scheme(scheme), 
    m_host(host), 
    m_port(port), 
    m_path(path), 
    m_query(query) {
}

char const* Url::getHttpScheme() {
    return "http";
}

char const* Url::getHttpsScheme() {
    return "https";
}

int Url::getDefaultPort() {
    return 80;
}

string const& Url::getScheme() const {
    return m_scheme;
}

string const& Url::getHost() const {
    return m_host;
}

int Url::getPort() const {
    return m_port;
}

string const& Url::getPath() const {
    return m_path;
}

string const& Url::getQuery() const {
    return m_query;
}

Url Url::parse(string const& urlString) {
    const string urlFormat = "((?<scheme>http(s)?)://)?(?<host>[^:/]+)(:(?<port>[\\d]+))?(?<path>(/[^/?]+)*)(\\?(?<query>.*))?";
    regex expression(urlFormat);
    smatch matches;
    bool found = regex_match(urlString, matches, expression);
    if (!found) {
        // throw bad url exception
    }
    string scheme = matches["scheme"];
    if (scheme == "") {
        scheme = Url::getHttpScheme();
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
    return Url(scheme, host, port, path, query);
}

}
