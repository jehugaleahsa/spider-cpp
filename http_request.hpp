#ifndef SPIDER_CPP_HTTP_REQUEST
#define SPIDER_CPP_HTTP_REQUEST

#include <memory>
#include <string>
#include <unordered_map>
#include <exception>
#include "header.hpp"
#include "http_response.hpp"
#include "url.hpp"

namespace spider {

     class HttpRequest {
        std::string m_method;
        Url m_url;
        HeaderCollection m_headers;

        static std::string const& getNewline();

    public:
        typedef std::shared_ptr<HttpResponse> response_ptr;

        HttpRequest(std::string const& method, Url const& url);

        HeaderCollection & getHeaders();

        HttpResponse getResponse() const;
     };

     class ConnectionException : public virtual std::exception {
        std::string m_what;

    public:
        ConnectionException(Url const& url) throw();
        virtual ~ConnectionException() throw();
        virtual char const* what() const throw();
     };

}

#endif // end SPIDER_CPP_HTTP_REQUEST
