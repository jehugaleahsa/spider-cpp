#ifndef SPIDER_HTTP_RESPONSE_INTERFACE_HPP
#define SPIDER_HTTP_RESPONSE_INTERFACE_HPP

#include <iosfwd>
#include <string>

namespace spider {

template <typename THeaderCollection>
class HttpResponseInterface {
protected:
    HttpResponseInterface() {
    }
   
public:
    virtual ~HttpResponseInterface() {
    }
    
    virtual std::string getVersion() = 0;
        
    virtual int getStatusCode() = 0;
    
    virtual std::string getStatusMessage() = 0;
    
    virtual THeaderCollection const& getHeaders() const = 0;

    //template <typename TOutIterator>
    //virtual void getHeaderNames(TOutIterator destination) = 0;

    //template <typename TOutIterator>
    //virtual void getHeaderValues(std::string const& name, TOutIterator destination) = 0;
    
    virtual std::istream & getContent() = 0;
};

}

#endif // SPIDER_HTTP_RESPONSE_INTERFACE_HPP
