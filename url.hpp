#ifndef SPIDER_URL
#define SPIDER_URL

#include <string>

namespace spider {

class Url {
    std::string m_scheme;
    std::string m_host;
    int m_port;
    std::string m_path;
    std::string m_query;
public:

    Url(std::string const& scheme,
        std::string const& host,
        int port,
        std::string const& path,
        std::string const& query);

    static char const* getHttpScheme();

    static char const* getHttpsScheme();

    static int getDefaultPort();

    std::string const& getScheme() const;

    std::string const& getHost() const;

    int getPort() const;

    std::string const& getPath() const;

    std::string const& getQuery() const;

    static Url parse(std::string const& urlString);
};

}

#endif // spider_url
