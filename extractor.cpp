#include <string>
#include <boost/regex.hpp>
#include "extractor.hpp"
#include "url.hpp"

namespace spider {

boost::regex const& UrlExtractor::getRegex() {
    using boost::regex;

    const static regex re("<a.*?href\\s*?=\\s*?(\"(?<url>.*?)\")|('(?<url>.*?)')");
    return re;
}

Url UrlExtractor::buildUrl(boost::smatch const& match) {
    using std::string;

    string urlString = match.str("url");
    try {
        return Url::parse(urlString);
    } catch (BadUrlException const& exception) {
        return m_baseAddress;
    }
}

UrlExtractor::UrlExtractor(std::string const& tagName, Url const& baseAddress)
    : m_tagName(tagName), m_baseAddress(baseAddress) {
}

}
