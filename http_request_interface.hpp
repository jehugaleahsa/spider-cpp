#ifndef SPIDER_HTTP_REQUEST_INTERFACE_HPP
#define SPIDER_HTTP_REQUEST_INTERFACE_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include "url.hpp"
#include "http_response_interface.hpp"

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

template <typename THeaderCollection>
class HttpRequestInterface {
protected:
    HttpRequestInterface() {
    }
    
public:
    
    virtual ~HttpRequestInterface() {
    }
    
    virtual THeaderCollection & getHeaders() = 0;

    virtual boost::shared_ptr<HttpResponseInterface<THeaderCollection> > getResponse() const = 0;
};

}

#endif // SPIDER_HTTP_REQUEST_INTERFACE_HPP
