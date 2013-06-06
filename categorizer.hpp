#ifndef SPIDER_CATEGORIZER_HPP
#define SPIDER_CATEGORIZER_HPP

#include <string>
#include <unordered_map>
#include "url.hpp"

namespace spider {

    class Categorizer {
        std::unordered_map<std::string, int> m_extensions;

    public:
        void supportExtension(int priority, std::string const& extension);

        bool isDesired(Url const& url) const;

        int getPriority(Url const& url) const;
    };

}


#endif // SPIDER_CATEGORIZER_HPP
