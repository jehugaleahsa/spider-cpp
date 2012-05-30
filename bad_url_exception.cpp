#include <exception>
#include <sstream>
#include <string>
#include "bad_url_exception.hpp"

namespace spider {

BadUrlException::BadUrlException() throw() {
}

BadUrlException::BadUrlException(std::string const& url) throw()
    : m_url(url) {
}

BadUrlException::~BadUrlException() throw() {
}

char const* BadUrlException::what() const throw() {
    using std::string;
    using std::ostringstream;
    using std::endl;

    ostringstream builder;
    builder << "The given string was not a valid URL: " << m_url << endl;
    string result = builder.str();
    return result.c_str();
}

}
