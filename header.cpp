#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "header.hpp"

namespace {

bool isHeaderLessThanName(spider::Header const& header, std::string const& name) {
    using std::less;
    using std::string;

    return less<string>()(header.getName(), name);
}

}

namespace spider {

Header::Header(std::string const& name) : m_name(name) {
}

void Header::addValue(std::string const& value) {
    m_values.push_back(value);
}

std::string Header::getName() const {
    return m_name;
}

Header::iterator Header::begin() const {
    return m_values.begin();
}

Header::iterator Header::end() const {
    return m_values.end();
}

std::ostream & operator<<(std::ostream & output, Header const& header) {
    using std::back_inserter;
    using std::copy;
    using std::ostringstream;
    using std::string;
    using std::vector;
    using boost::join;

    output << header.getName() << ": ";
    vector<string> values;
    copy(header.begin(), header.end(), back_inserter(values));
    output << join(values, ";");
    return output;
}

void HeaderCollection::addHeader(std::string const& name, std::string const& value) {
    using std::lower_bound;
    using std::vector;

    vector<Header>::iterator position = lower_bound(
        m_headers.begin(), m_headers.end(), name, isHeaderLessThanName);
    if (position == m_headers.end() || position->getName() != name) {
        position = m_headers.insert(position, Header(name));
    }
    position->addValue(value);
}

HeaderCollection::header_ptr HeaderCollection::getHeader(std::string const& name) const {
    using std::lower_bound;
    using std::vector;

    vector<Header>::const_iterator position = lower_bound(
        m_headers.begin(), m_headers.end(), name, isHeaderLessThanName);
    if (position == m_headers.end() || position->getName() != name) {
        return header_ptr();
    }
    return &*position;
}

HeaderCollection::iterator HeaderCollection::begin() const {
    return m_headers.begin();
}

HeaderCollection::iterator HeaderCollection::end() const {
    return m_headers.end();
}

}
