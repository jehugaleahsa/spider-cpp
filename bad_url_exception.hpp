#ifndef SPIDER_BAD_URL_EXCEPTION_HPP
#define SPIDER_BAD_URL_EXCEPTION_HPP

#include <string>
#include <exception>

namespace spider {

class BadUrlException : public virtual std::exception {
    std::string m_url;
public:
    BadUrlException() throw();
    BadUrlException(std::string const& url) throw();
    virtual ~BadUrlException() throw();
    virtual char const* what() const throw();
};

}

#endif // end SPIDER_BAD_URL_EXCEPTION_HPP
