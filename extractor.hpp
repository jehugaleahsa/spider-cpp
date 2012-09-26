#ifndef SPIDER_EXTRACTOR_HPP
#define SPIDER_EXTRACTOR_HPP

#include <algorithm>
#include <string>
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include "url.hpp"

namespace spider {

class UrlExtractor {
    std::string m_tagName;
    Url m_baseAddress;

    boost::regex const& getRegex();

    Url buildUrl(boost::smatch const& match);

public:
    explicit UrlExtractor(std::string const& tagName, Url const& baseAddress);

    template <typename TOutputIterator>
    TOutputIterator getUrls(std::string const& content, TOutputIterator dest);
};

template <typename TOutputIterator>
TOutputIterator UrlExtractor::getUrls(
    std::string const& content,
    TOutputIterator dest) {
    using std::transform;
    using boost::bind;
    using boost::regex;
    using boost::sregex_iterator;

    regex const& re = UrlExtractor::getRegex();
    sregex_iterator regex_begin = make_regex_iterator(content, re);
    sregex_iterator regex_end;
    return transform(
        regex_begin, regex_end,
        dest,
        bind(&UrlExtractor::buildUrl, this, _1));
}

}

#endif // SPIDER_EXTRACTOR_HPP
