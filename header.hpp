#ifndef SPIDER_HEADER_HPP
#define SPIDER_HEADER_HPP

#include <iterator>
#include <map>
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
    std::string getName() const;
    
    std::vector<std::string>::const_iterator begin() const;
    
    std::vector<std::string>::const_iterator end() const;
};

class HeaderIterator : public std::iterator<std::bidirectional_iterator_tag, Header> {
    friend class HeaderCollection;
    
    std::map<std::string, Header>::const_iterator m_iterator;
    
    HeaderIterator(std::map<std::string, Header>::const_iterator iterator);
    
public:    
    HeaderIterator(HeaderIterator const& other);
    
    inline HeaderIterator & operator =(HeaderIterator const& other);
    
    inline Header const& operator *();
    
    inline Header const* operator ->();
    
    inline HeaderIterator & operator ++();
    
    inline HeaderIterator & operator --();
    
    inline bool operator ==(HeaderIterator const& other) const;
};

HeaderIterator & HeaderIterator::operator =(HeaderIterator const& other) {
    if (this != &other) {
        m_iterator = other.m_iterator;
    }
    return *this;
}

Header const& HeaderIterator::operator *() {
    return m_iterator->second;
}

Header const* HeaderIterator::operator ->() {
    return &m_iterator->second;
}

HeaderIterator & HeaderIterator::operator ++() {
    ++m_iterator;
    return *this;
}

HeaderIterator & HeaderIterator::operator --() {
    --m_iterator;
    return *this;
}

bool HeaderIterator::operator ==(HeaderIterator const& other) const {
    return m_iterator == other.m_iterator;
}

inline HeaderIterator operator ++(HeaderIterator & iterator, int) {
    HeaderIterator temp = iterator;
    ++iterator;
    return temp;
}

inline HeaderIterator operator --(HeaderIterator & iterator, int) {
    HeaderIterator temp = iterator;
    --iterator;
    return temp;
}

inline bool operator !=(HeaderIterator const& first, HeaderIterator const& second) {
    return !(first == second);
}

class HeaderCollection {
    std::map<std::string, Header> m_headers;
    
public:
    void addHeader(std::string const& name, std::string const& value);
    
    bool hasHeader(std::string const& name) const;
    
    Header const& getHeader(std::string const& name) const;
  
    HeaderIterator const begin() const;
  
    HeaderIterator const end() const;
};

}

#endif // SPIDER_HEADER_HPP
