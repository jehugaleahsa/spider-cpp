#ifndef SPIDER_CATEGORIZER_HPP
#define SPIDER_CATEGORIZER_HPP

#include <string>
#include <unordered_map>
#include "url.hpp"

namespace spider {

    class Categorizer {
        std::unordered_map<std::string, int> m_extensions;

    public:
        virtual ~Categorizer();

        virtual bool isDesired(Url const& url) const;

        void supportExtension(int priority, std::string const& extension);

        int getPriority(Url const& url) const;
    };

    class DomainCategorizer : public virtual Categorizer {
        std::string m_domain;

    public:
        DomainCategorizer(std::string const& domain);

        bool isDesired(Url const& url) const override;
    };

}


#endif // SPIDER_CATEGORIZER_HPP
