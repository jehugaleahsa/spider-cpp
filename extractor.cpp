#include <sstream>
#include <string>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/regex.hpp>
#include "extractor.hpp"
#include "url.hpp"

namespace {

boost::regex getRegex(
    std::string const& tagName,
    std::string const& attributeName) {
    using std::stringstream;
    using boost::regex;

    stringstream builder;
    builder << '<';
    builder << tagName;
    builder << ".*?";
    builder << attributeName;
    builder << "\\s*?=\\s*?((\"(?<url>.*?)\")|('(?<url>.*?)'))";
    regex re(builder.str(), boost::regex::icase);
    return re;
}

}

namespace spider {

Url UrlExtractor::buildUrl(
    Url const& baseAddress,
    boost::smatch const& match) const {
    using std::ostringstream;
    using std::string;
    using boost::istarts_with;

    string urlString = match.str("url");
    if (istarts_with(urlString, "javascript:")) {
        return baseAddress;
    }
    if (!istarts_with(urlString, "http://")
        && !istarts_with(urlString, "https://")) {
        ostringstream builder;
        builder << baseAddress;
        builder << urlString;
        urlString = builder.str();
    }
    try {
        return Url::parse(urlString);
    } catch (BadUrlException const& exception) {
        return baseAddress;
    }
}

UrlExtractor::UrlExtractor(
    std::string const& tagName,
    std::string const& attributeName)
    : m_regex(getRegex(tagName, attributeName)) {
}

}
