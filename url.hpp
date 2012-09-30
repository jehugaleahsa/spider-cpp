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
    std::string m_userInfo;
    std::string m_host;
    int m_port;
    std::string m_path;
    std::string m_query;
    std::string m_fragment;

public:
    Url(std::string const& host,
        int port = Url::getDefaultPort(),
        std::string const& path = "",
        std::string const& query = "",
        std::string const& fragment = "",
        std::string const& scheme = getDefaultScheme(),
        std::string const& userInfo = "");

    static std::string const& getDefaultScheme();

    static int getDefaultPort();

    std::string const& getScheme() const;

    std::string const& getUserInfo() const;

    std::string const& getHost() const;

    int getPort() const;

    std::string const& getPath() const;

    std::string const& getQuery() const;

    std::string const& getFragment() const;

    static Url parse(std::string const& urlString);
};

inline std::string const& Url::getScheme() const {
    return m_scheme;
}

inline std::string const& Url::getUserInfo() const {
    return m_userInfo;
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

inline std::string const& Url::getFragment() const {
    return m_fragment;
}

std::ostream & operator<<(std::ostream & stream, Url const& url);

class BadUrlException : public virtual std::exception {
    std::string m_what;

public:
    BadUrlException(std::string const& url) throw();
    virtual ~BadUrlException() throw();
    virtual char const* what() const throw();
};

}

namespace std {

template <>
struct equal_to<spider::Url> : public binary_function<spider::Url, spider::Url, bool> {
    result_type operator ()(first_argument_type const& first, second_argument_type const& second) const {
        using std::equal_to;
        using std::string;

        return equal_to<string>()(first.getScheme(), second.getScheme())
            && equal_to<string>()(first.getUserInfo(), second.getUserInfo())
            && equal_to<string>()(first.getHost(), second.getHost())
            && equal_to<int>()(first.getPort(), second.getPort())
            && equal_to<string>()(first.getPath(), second.getPath())
            && equal_to<string>()(first.getQuery(), second.getQuery());
            //&& equal_to<string>()(first.getFragment(), second.getFragment());
    }
};

}

namespace boost {

template <>
struct hash<spider::Url> : public std::unary_function<spider::Url, std::size_t> {
    result_type operator ()(argument_type const& url) const {
        using std::string;

        return hash<string>()(url.getScheme())
            ^ hash<string>()(url.getUserInfo())
            ^ hash<string>()(url.getHost())
            ^ hash<int>()(url.getPort())
            ^ hash<string>()(url.getPath())
            ^ hash<string>()(url.getQuery());
            //^ hash<string>()(url.getFragment());
    }
};

}

#endif // SPIDER_URL
