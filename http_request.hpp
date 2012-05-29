#ifndef SPIDER_CPP_HTTP_REQUEST
#define SPIDER_CPP_HTTP_REQUEST

#include <string>
#include <boost/unordered_map.hpp>
#include "url.hpp"

namespace spider {

enum RequestMethod { 
    GET, 
    POST, 
    PUT, 
    DELETE 
};

std::string str(RequestMethod method);

class HttpRequest
{
    RequestMethod m_method;
    Url m_url;
    boost::unordered_map<std::string, std::string> m_headers;

    static std::string getNewline();

public:
    HttpRequest(RequestMethod method, Url const& url);

    void setHeader(std::string const& name, std::string const& value);

    void getResponse() const;
};

}

#endif // end SPIDER_CPP_HTTP_REQUEST
