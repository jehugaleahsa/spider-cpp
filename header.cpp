#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>
#include "header.hpp"

spider::Header::Header(std::string const& name) : m_name(name) {
}

void spider::Header::addValue(std::string const& value) {
    m_values.push_back(value);
}

std::string spider::Header::getName() const {
    return m_name;
}

std::string const& spider::Header::getValue(size_type index) const {
    return m_values[index];
}

std::ostream & spider::operator<<(std::ostream & output, Header const& header) {
    using std::back_inserter;
    using std::copy;
    using std::ostringstream;
    using std::string;
    using std::vector;
    using boost::join;

    output << header.getName() << ": ";
    vector<string> values;
    header.getValues(back_inserter(values));
    output << join(values, ";");
    return output;
}

void spider::HeaderCollection::addHeader(
    std::string const& name, 
    std::string const& value) {
    using std::pair;
    using std::string;
    using boost::unordered_map;

    unordered_map<string, Header>::iterator position = m_headers.find(name);
    if (position == m_headers.end()) {
        position = m_headers.insert(
            position, pair<string const, Header>(name, Header(name)));
    }
    position->second.addValue(value);
}

bool spider::HeaderCollection::hasHeader(std::string const& name) const {
    return m_headers.find(name) != m_headers.end();
}

spider::Header const& spider::HeaderCollection::getHeader(std::string const& name) const {
    using std::string;
    using boost::unordered_map;

    unordered_map<string, Header>::const_iterator position = m_headers.find(name);
    if (position == m_headers.end()) {
        throw HeaderNotFoundException(name);
    }
    return position->second;
}

spider::HeaderNotFoundException::HeaderNotFoundException(
    std::string const& headerName) throw() {
    using std::ostringstream;

    ostringstream builder;
    builder << "The given header did not exist: " << headerName << ".";
    m_what = builder.str();
}

spider::HeaderNotFoundException::~HeaderNotFoundException() throw() {
}

char const* spider::HeaderNotFoundException::what() const throw() {
    return m_what.c_str();
}
