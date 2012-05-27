#include <string>
#include "url.hpp"

namespace spider {

Url::Url(std::string const& host)
    : m_scheme("http"), m_host(host), m_port(80), m_path(), m_queryString() {
}

Url Url::parse(std::string const& urlString) {
    Url url("www.google.com");
    return url; 
}

}
