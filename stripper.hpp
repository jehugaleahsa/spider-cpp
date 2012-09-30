#ifndef SPIDER_STRIPPER_HPP
#define SPIDER_STRIPPER_HPP

#include <string>
#include <boost/regex.hpp>

namespace spider {

    class Stripper {
        boost::regex m_regex;

    public:
        Stripper(std::string const& tagName);

        std::string strip(std::string const& content) const;
    };

}

#endif // SPIDER_STRIPPER_HPP
