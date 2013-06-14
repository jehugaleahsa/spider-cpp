#include <string>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include "categorizer.hpp"
#include "url.hpp"

namespace {

    std::string getExtension(spider::Url const& url) {
        using std::string;
        using boost::to_lower;

        string const& path = url.getPath();
        string::const_reverse_iterator rposition = find(
            path.rbegin(), path.rend(), '.');

        if (rposition == path.rend()) {
            return string();
        }
        string::const_iterator position = rposition.base();
        string extension = string(position, path.end());
        to_lower(extension);
        return extension;
    }

}

spider::Categorizer::~Categorizer() {
}

void spider::Categorizer::supportExtension(int priority, std::string const& extension) {
    using std::string;
    using boost::to_lower_copy;

    string copy = to_lower_copy(extension);
    m_extensions[copy] = priority;
}

bool spider::Categorizer::isDesired(Url const& url) const {
    using std::find;
    using std::string;
    using boost::to_lower;

    string extension = getExtension(url);
    return m_extensions.find(extension) != m_extensions.end();
}

int spider::Categorizer::getPriority(Url const& url) const {
    using std::string;
    using std::unordered_map;

    string extension = getExtension(url);
    auto iterator = m_extensions.find(extension);
    if (iterator == m_extensions.end()) {
        return 0;
    }
    return iterator->second;
}

spider::DomainCategorizer::DomainCategorizer(std::string const& domain) 
    : m_domain(domain) {
}

bool spider::DomainCategorizer::isDesired(Url const& url) const {
    using std::string;
    using boost::iends_with;

    string const& domain = url.getHost();
    return iends_with(domain, m_domain) && Categorizer::isDesired(url);
}
