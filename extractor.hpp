#ifndef SPIDER_EXTRACTOR_HPP
#define SPIDER_EXTRACTOR_HPP

#include <algorithm>
#include <string>
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include "url.hpp"

namespace spider {

class UrlExtractor {
    boost::regex m_regex;

    Url buildUrl(Url const& baseAddress, boost::smatch const& match) const;

public:
    explicit UrlExtractor(
        std::string const& tagName,
        std::string const& attributeName);

    template <typename TOutputIterator>
    TOutputIterator getUrls(
        Url const& baseAddress,
        std::string const& content,
        TOutputIterator dest) const;
};

template <typename TOutputIterator>
TOutputIterator UrlExtractor::getUrls(
    Url const& baseAddress,
    std::string const& content,
    TOutputIterator dest) const {
    using std::transform;
    using boost::bind;
    using boost::sregex_iterator;

    sregex_iterator regex_begin = make_regex_iterator(content, m_regex);
    sregex_iterator regex_end;
    return transform(
        regex_begin, regex_end,
        dest,
        bind(&UrlExtractor::buildUrl, this, baseAddress, _1));
}

}

#endif // SPIDER_EXTRACTOR_HPP
