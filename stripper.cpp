#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include "stripper.hpp"

namespace {

    boost::regex getRegex(std::string const& tagName) {
        using std::ostringstream;
        using boost::regex;

        ostringstream builder;
        builder << '<';
        builder << tagName;
        builder << ".*?>.*?</";
        builder << tagName;
        builder << "\\s*?>";
        regex re(builder.str(), boost::regex::icase);
        return re;
    }

}

namespace spider {

    Stripper::Stripper(std::string const& tagName)
        : m_regex(getRegex(tagName)) {
    }

    std::string Stripper::strip(std::string const& content) const {
        using boost::regex_replace;

        return regex_replace(content, m_regex, "");
    }

}
