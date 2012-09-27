#ifndef SPIDER_SPIDER_HPP
#define SPIDER_SPIDER_HPP

#include <iosfwd>
#include "url.hpp"

namespace spider {

class Spider {
public:
    void run(std::ostream & output, Url const& topUrl) const;
};

}

#endif // SPIDER_SPIDER_HPP
