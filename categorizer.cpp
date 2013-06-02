#include <string>
#include <boost/algorithm/string.hpp>
#include "categorizer.hpp"
#include "url.hpp"

void spider::Categorizer::supportExtension(std::string const& extension) {
    using std::string;
    using boost::to_lower;

    string copy(extension);
    to_lower(copy);
    m_extensions.insert(copy);
}

bool spider::Categorizer::isDesired(Url const& url) const {
    using std::find;
    using std::string;
    using boost::to_lower;

    string const& path = url.getPath();
    string::const_reverse_iterator rposition = find(
        path.rbegin(), path.rend(), '.');

    string extension;
    if (rposition != path.rend()) {
        string::const_iterator position = rposition.base();
        extension = string(position, path.end());
        to_lower(extension);
    }
    return m_extensions.find(extension) != m_extensions.end();
}
