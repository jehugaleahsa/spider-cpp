#ifndef SPIDER_URL
#define SPIDER_URL

#include <string>

class Url {
    std::string m_scheme;
    std::string m_host;
    int m_port;
    std::string m_path;
    std::string m_queryString;
public:
    Url(std::string const& host);
};

#endif // spider_url
