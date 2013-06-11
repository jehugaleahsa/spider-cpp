#ifndef SPIDER_SPIDER_HPP
#define SPIDER_SPIDER_HPP

#include <iosfwd>
#include <string>
#include "categorizer.hpp"
#include "url.hpp"

namespace spider {

    class Spider {
    public:
        void run(
            Url const& topUrl, 
            std::string const& downloadDirectory,
            Categorizer const& pageCategorizer,
            Categorizer const& mediaCategorizer) const;
    };

}

#endif // SPIDER_SPIDER_HPP
