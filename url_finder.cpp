#include <string>
#include <vector>
#include "extractor.hpp"
#include "stripper.hpp"
#include "url_finder.hpp"
#include "url.hpp"

spider::Url spider::UrlFinder::getBaseUrl(Url const& url, std::string const& content) const {
    using std::back_inserter;
    using std::vector;

    vector<Url> baseAddresses;
    m_baseExtractor.getUrls(url, content, baseAddresses);
    if (baseAddresses.empty()) {
        return url;
    } else {
        return baseAddresses.back();
    }
}

spider::UrlFinder::UrlFinder(
    Stripper const& stripper,
    UrlExtractor const& baseExtractor,
    UrlExtractor const& extractor)
:
    m_stripper(stripper),
    m_baseExtractor(baseExtractor),
    m_extractor(extractor) {
}
