#include <string>
#include "url.hpp"

Url::Url(std::string const& host)
    : m_scheme("http"), m_host(host), m_port(80), m_path(), m_queryString() {
}
