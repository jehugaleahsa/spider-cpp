#ifndef SPIDER_URL
#define SPIDER_URL

#include <string>

namespace spider {

class Url {
    std::string m_scheme;
    std::string m_host;
    int m_port;
    std::string m_path;
    std::string m_queryString;
public:
    Url(std::string const& host);

    static Url parse(std::string const& urlString);
};

}

#endif // spider_url
