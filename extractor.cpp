#include <sstream>
#include <string>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/regex.hpp>
#include "extractor.hpp"
#include "url.hpp"

namespace {

boost::regex getRegex(std::string const& tagName) {
    using std::stringstream;
    using boost::regex;

    stringstream builder;
    builder << '<';
    builder << tagName;
    builder << ".*?href\\s*?=\\s*?((\"(?<url>.*?)\")|('(?<url>.*?)'))";
    regex re(builder.str(), boost::regex::icase);
    return re;
}

}

namespace spider {

Url UrlExtractor::buildUrl(boost::smatch const& match) const {
    using std::string;
    using std::stringstream;
    using boost::istarts_with;

    string urlString = match.str("url");
    if (istarts_with(urlString, "javascript:void(0)")) {
        return m_baseAddress;
    }
    if (!istarts_with(urlString, "http://")
        && !istarts_with(urlString, "https://")) {
        stringstream builder;
        builder << m_baseAddress;
        builder << urlString;
        urlString = builder.str();
    }
    try {
        return Url::parse(urlString);
    } catch (BadUrlException const& exception) {
        return m_baseAddress;
    }
}

UrlExtractor::UrlExtractor(std::string const& tagName, Url const& baseAddress)
    : m_regex(getRegex(tagName)), m_baseAddress(baseAddress) {
}

}
