#ifndef SPIDER_CATEGORIZER_HPP
#define SPIDER_CATEGORIZER_HPP

#include <string>
#include <boost/unordered_set.hpp>
#include "url.hpp"

namespace spider {

class Categorizer {
    boost::unordered_set<std::string> m_extensions;

public:
    void supportExtension(std::string const& extension);

    bool isDesired(Url const& url) const;
};

}


#endif // SPIDER_CATEGORIZER_HPP
