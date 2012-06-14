#include <map>
#include <string>
#include <vector>
#include "header.hpp"

namespace spider {

Header::Header(std::string const& name) : m_name(name) {
}

void Header::addValue(std::string const& value) {
    m_values.push_back(value);
}

std::string Header::getName() const {
    return m_name;
}

std::vector<std::string>::const_iterator Header::begin() const {
    return m_values.begin();
}

std::vector<std::string>::const_iterator Header::end() const {
    return m_values.end();
}

HeaderIterator::HeaderIterator(std::map<std::string, Header>::const_iterator iterator)
    : m_iterator(iterator) {
}

HeaderIterator::HeaderIterator(HeaderIterator const& other)
    : m_iterator(other.m_iterator) {
}

void HeaderCollection::addHeader(std::string const& name, std::string const& value) {
    using std::map;
    using std::string;
    
    map<string, Header>::iterator position = m_headers.find(name);
    if (position == m_headers.end()) {
        position = m_headers.insert(m_headers.end(), make_pair(name, Header(name)));
    }
    position->second.addValue(value);
}

bool HeaderCollection::hasHeader(std::string const& name) const {
    return m_headers.find(name) != m_headers.end();
}

Header const& HeaderCollection::getHeader(std::string const& name) const {
    using std::map;
    using std::string;
    
    map<string, Header>::const_iterator position = m_headers.find(name);
    if (position == m_headers.end()) {
        // TODO: create an exception for missing headers
        throw "No such header!";
    }
    return position->second;
}

HeaderIterator const HeaderCollection::begin() const {
    return HeaderIterator(m_headers.begin());
}

HeaderIterator const HeaderCollection::end() const {
    return HeaderIterator(m_headers.end());
}

}
