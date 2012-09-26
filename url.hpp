#ifndef SPIDER_URL_HPP
#define SPIDER_URL_HPP

#include <cstddef>
#include <exception>
#include <functional>
#include <iosfwd>
#include <string>
#include <boost/functional/hash.hpp>

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

    static std::string const& getDefaultScheme();

    static int getDefaultPort();

    std::string const& getScheme() const;

    std::string const& getHost() const;

    int getPort() const;

    std::string const& getPath() const;

    std::string const& getQuery() const;

    static Url parse(std::string const& urlString);
};

inline std::string const& Url::getScheme() const {
    return m_scheme;
}

inline std::string const& Url::getHost() const {
    return m_host;
}

inline int Url::getPort() const {
    return m_port;
}

inline std::string const& Url::getPath() const {
    return m_path;
}

inline std::string const& Url::getQuery() const {
    return m_query;
}

std::ostream & operator<<(std::ostream & stream, Url const& url);

class BadUrlException : public virtual std::exception {
    std::string m_url;
public:
    BadUrlException() throw();
    BadUrlException(std::string const& url) throw();
    virtual ~BadUrlException() throw();
    virtual char const* what() const throw();
};

}

namespace std {

template <>
struct equal_to<spider::Url> : public binary_function<spider::Url, spider::Url, bool> {
    result_type operator ()(first_argument_type const& first, second_argument_type const& second) const {
        return std::equal_to<std::string>()(first.getScheme(), second.getScheme())
            && std::equal_to<std::string>()(first.getHost(), second.getHost())
            && std::equal_to<int>()(first.getPort(), second.getPort())
            && std::equal_to<std::string>()(first.getPath(), second.getPath())
            && std::equal_to<std::string>()(first.getQuery(), second.getQuery());
    }
};

}

namespace boost {

template <>
struct hash<spider::Url> : public std::unary_function<spider::Url, std::size_t> {
    result_type operator ()(argument_type const& url) const {
        return hash<std::string>()(url.getScheme())
            ^ hash<std::string>()(url.getHost())
            ^ hash<int>()(url.getPort())
            ^ hash<std::string>()(url.getPath())
            ^ hash<std::string>()(url.getQuery());
    }
};

}

#endif // SPIDER_URL
