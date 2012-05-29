#include <exception>
#include <sstream>
#include <string>
#include "bad_url_exception.hpp"

using namespace std;

namespace spider {

BadUrlException::BadUrlException() throw() {
}

BadUrlException::BadUrlException(string const& url) throw()
    : m_url(url) {
}

BadUrlException::~BadUrlException() throw() {
}

char const* BadUrlException::what() const throw() {
    ostringstream builder;
    builder << "The given string was not a valid URL: " << m_url << endl;
    string result = builder.str();
    return result.c_str();
}

}
