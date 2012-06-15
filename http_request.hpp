#ifndef SPIDER_CPP_HTTP_REQUEST
#define SPIDER_CPP_HTTP_REQUEST

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include "header.hpp"
#include "http_response.hpp"
#include "url.hpp"

namespace spider {

enum RequestMethod { 
    GET, 
    POST, 
    PUT, 
    DELETE,
    HEAD,
    TRACE,
    CONNECT,
};

class HttpRequest {
    RequestMethod m_method;
    Url m_url;
    HeaderCollection m_headers;

    static std::string const& getNewline();

public:
    typedef boost::shared_ptr<HttpResponse> response_ptr;

    HttpRequest(RequestMethod method, Url const& url);

    //void setHeader(std::string const& name, std::string const& value);
    
    HeaderCollection & getHeaders();

    response_ptr getResponse() const;
};

}

#endif // end SPIDER_CPP_HTTP_REQUEST
