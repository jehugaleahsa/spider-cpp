#ifndef SPIDER_HEADER_HPP
#define SPIDER_HEADER_HPP

#include <string>
#include <vector>
#include <boost/unordered_map.hpp>

namespace spider {

class Header {
    friend class HeaderCollection;

    std::string m_name;
    std::vector<std::string> m_values;

    Header(std::string const& name);

    void addValue(std::string const& value);

public:
    typedef std::vector<std::string>::size_type size_type;

    std::string getName() const;

    inline size_type getValueCount() const;

    template <typename TOutputIterator>
    TOutputIterator getValues(TOutputIterator destination) const;
};

std::ostream & operator<<(std::ostream & output, Header const& header);

Header::size_type Header::getValueCount() const {
    return m_values.size();
}

template <typename TOutputIterator>
TOutputIterator Header::getValues(TOutputIterator destination) const {
    using std::copy;

    return copy(m_values.begin(), m_values.end(), destination);
}

}

namespace {

spider::Header const& getConstHeader(
    std::pair<std::string const, spider::Header> const& pair) {
    return pair.second;
}

spider::Header & getHeader(std::pair<std::string const, spider::Header> & pair) {
    using spider::Header;

    return const_cast<Header&>(getConstHeader(pair));
}

}

namespace spider {

class HeaderCollection {
    boost::unordered_map<std::string, Header> m_headers;

public:
    typedef boost::unordered_map<std::string, Header>::size_type size_type;

    void addHeader(std::string const& name, std::string const& value);

    bool hasHeader(std::string const& name) const;

    Header const& getHeader(std::string const& name) const;

    inline size_type getHeaderCount() const;

    template <typename TOutputIterator>
    TOutputIterator getHeaders(TOutputIterator destination);

    template <typename TOutputIterator>
    TOutputIterator getHeaders(TOutputIterator destination) const;
};

HeaderCollection::size_type HeaderCollection::getHeaderCount() const {
    return m_headers.size();
}

template <typename TOutputIterator>
TOutputIterator HeaderCollection::getHeaders(TOutputIterator destination) {
    using std::transform;

    return transform(
        m_headers.begin(), m_headers.end(),
        destination,
        ::getHeader);
}

template <typename TOutputIterator>
TOutputIterator HeaderCollection::getHeaders(TOutputIterator destination) const {
    using std::transform;

    return transform(
        m_headers.begin(), m_headers.end(),
        destination,
        ::getConstHeader);
}

class HeaderNotFoundException : public virtual std::exception {
    std::string m_what;

public:
    HeaderNotFoundException(std::string const& headerName) throw();

    virtual ~HeaderNotFoundException() throw();

    char const* what() const throw();
};

}

#endif // SPIDER_HEADER_HPP
