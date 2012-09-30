#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>
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

    std::ostream & operator<<(std::ostream & output, Header const& header) {
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

    void HeaderCollection::addHeader(std::string const& name, std::string const& value) {
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

    bool HeaderCollection::hasHeader(std::string const& name) const {
        return m_headers.find(name) != m_headers.end();
    }

    Header const& HeaderCollection::getHeader(std::string const& name) const {
        using std::string;
        using boost::unordered_map;

        unordered_map<string, Header>::const_iterator position = m_headers.find(name);
        if (position == m_headers.end()) {
            throw HeaderNotFoundException(name);
        }
        return position->second;
    }

    HeaderNotFoundException::HeaderNotFoundException(
        std::string const& headerName) throw() {
        using std::ostringstream;

        ostringstream builder;
        builder << "The given header did not exist: " << headerName << ".";
        m_what = builder.str();
    }

    HeaderNotFoundException::~HeaderNotFoundException() throw() {
    }

    char const* HeaderNotFoundException::what() const throw() {
        return m_what.c_str();
    }

}
