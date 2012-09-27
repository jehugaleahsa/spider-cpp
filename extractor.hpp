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
    Url m_baseAddress;

    Url buildUrl(boost::smatch const& match) const;

public:
    explicit UrlExtractor(std::string const& tagName, Url const& baseAddress);

    template <typename TOutputIterator>
    TOutputIterator getUrls(std::string const& content, TOutputIterator dest) const;
};

template <typename TOutputIterator>
TOutputIterator UrlExtractor::getUrls(
    std::string const& content,
    TOutputIterator dest) const {
    using std::transform;
    using boost::bind;
    using boost::regex;
    using boost::sregex_iterator;

    sregex_iterator regex_begin = make_regex_iterator(content, m_regex);
    sregex_iterator regex_end;
    return transform(
        regex_begin, regex_end,
        dest,
        bind(&UrlExtractor::buildUrl, this, _1));
}

}

#endif // SPIDER_EXTRACTOR_HPP
