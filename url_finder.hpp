#ifndef SPIDER_URL_FINDER_HPP
#define SPIDER_URL_FINDER_HPP

#include <string>
#include <vector>
#include "extractor.hpp"
#include "stripper.hpp"
#include "url.hpp"

namespace spider {

    class UrlFinder {
        Stripper const& m_stripper;
        UrlExtractor const& m_baseExtractor;
        UrlExtractor const& m_extractor;

        Url getBaseUrl(Url const& url, std::string const& content) const;

    public:
        UrlFinder(
            Stripper const& stripper,
            UrlExtractor const& baseExtractor,
            UrlExtractor const& extractor);

        template <typename TOutputIterator>
        TOutputIterator getUrls(
            Url const& url, 
            std::string const& content, 
            TOutputIterator output) const;
    };

    template <typename TOutputIterator>
    TOutputIterator UrlFinder::getUrls(
        Url const& url, 
        std::string const& content, 
        TOutputIterator output) const {
        using std::copy;
        using std::string;
        using std::vector;

        string stripped = m_stripper.strip(content);
        Url baseUrl = getBaseUrl(url, stripped);
        vector<Url> urls;
        m_extractor.getUrls(baseUrl, stripped, urls);
        return copy(urls.begin(), urls.end(), output);
    }

}

#endif  // SPIDER_URL_FINDER_HPP
