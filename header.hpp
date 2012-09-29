#ifndef SPIDER_HEADER_HPP
#define SPIDER_HEADER_HPP

#include <string>
#include <vector>

namespace spider {

class Header {
    friend class HeaderCollection;

    std::string m_name;
    std::vector<std::string> m_values;

    Header(std::string const& name);

    void addValue(std::string const& value);

public:
    typedef std::vector<std::string>::const_iterator iterator;

    std::string getName() const;

    iterator begin() const;

    iterator end() const;
};

std::ostream & operator<<(std::ostream & output, Header const& header);

class HeaderCollection {
    std::vector<Header> m_headers;

public:
    typedef std::vector<Header>::const_iterator iterator;
    typedef Header const* header_ptr;

    void addHeader(std::string const& name, std::string const& value);

    header_ptr getHeader(std::string const& name) const;

    iterator begin() const;

    iterator end() const;
};

}

#endif // SPIDER_HEADER_HPP
