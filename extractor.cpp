#include <functional>
#include <memory>
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
        regex re(builder.str(), regex::icase);
        return re;
    }

}

spider::UrlExtractor::UrlExtractor(){
}

spider::UrlExtractor::~UrlExtractor() {
}

spider::Url spider::TagUrlExtractor::buildUrl(
    Url const& baseAddress,
    boost::smatch const& match) const {
    using std::ostringstream;
    using std::string;
    using boost::istarts_with;

    string urlString = match.str("url");
    // skip blank links
    if (urlString == "") {
        return baseAddress;
    }
    // ignore javascript links
    if (istarts_with(urlString, "javascript:")) {
        return baseAddress;
    }
    // handle shortened paths
    if (!istarts_with(urlString, "http://")
        && !istarts_with(urlString, "https://")) {
        // we don't care about fragment identifiers
        if (urlString[0] == '#') {
            return baseAddress;
        // handle semi-absolute URLs
        } else if (urlString[0] == '/') {
            Url forAbsolute(
                baseAddress.getHost(),
                baseAddress.getPort(),
                "",  // path
                "",  // query
                "",  // fragment
                baseAddress.getScheme(),
                baseAddress.getUserInfo());
            ostringstream builder;
            builder << forAbsolute;
            builder << urlString;
            urlString = builder.str();
        // handle relative URLs
        } else {
            Url forRelative(
                baseAddress.getHost(),
                baseAddress.getPort(),
                baseAddress.getPath(),
                "",  // query
                "",  // fragment
                baseAddress.getScheme(),
                baseAddress.getUserInfo());
            ostringstream builder;
            builder << forRelative;
            builder << urlString;
            urlString = builder.str();
        }
    }
    try {
        return Url::parse(urlString);
    } catch (BadUrlException const& exception) {
        std::cerr << exception.what() << std::endl;
        return baseAddress;
    }
}

void spider::TagUrlExtractor::getUrls(
    Url const& baseAddress,
    std::string const& content, 
    std::vector<Url> & destination) const {
    using std::back_inserter;
    using std::transform;
    using boost::smatch;
    using boost::sregex_iterator;

    sregex_iterator regex_begin(content.begin(), content.end(), m_regex);
    sregex_iterator regex_end;
    transform(
        regex_begin, regex_end,
        back_inserter(destination),
        [&](smatch const& match) {
            return buildUrl(baseAddress, match);
        });
}


spider::TagUrlExtractor::TagUrlExtractor(
    std::string const& tagName, std::string const& attributeName)
    : m_regex(getRegex(tagName, attributeName)) {
}

spider::TagUrlExtractor::TagUrlExtractor(TagUrlExtractor const& other) 
    : m_regex(other.m_regex) {
}

void spider::CompoundExtractor::addExtractor(std::shared_ptr<UrlExtractor> extractor) {
    m_extractors.push_back(extractor);
}

void spider::CompoundExtractor::getUrls(
    Url const& baseAddress,
    std::string const& content,
    std::vector<Url> & destination) const {
    using std::for_each;
    using std::shared_ptr;

    for_each(
        m_extractors.begin(), m_extractors.end(),
            [&](shared_ptr<UrlExtractor> extractor) {
                extractor->getUrls(baseAddress, content, destination);
            });
}
