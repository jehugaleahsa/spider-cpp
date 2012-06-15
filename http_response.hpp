#ifndef SPIDER_HTTP_RESPONSE_HPP
#define SPIDER_HTTP_RESPONSE_HPP

#include <iosfwd>
#include <set>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include "header.hpp"

namespace spider {

    class HttpResponse {
    private:
        friend class HttpRequest;

        boost::shared_ptr<std::istream> m_stream;

        bool m_isInitialized;

        bool m_hasStatus;
        void getStatusCached();
        std::string m_version;
        int m_statusCode;
        std::string m_statusMessage;

        bool m_hasHeaders;
        void getHeadersCached();
        HeaderCollection m_headers;

        HttpResponse(boost::shared_ptr<std::istream> stream);

        bool readLine(std::string & line);

    public:
        std::string getVersion();
        
        int getStatusCode();
        
        std::string getStatusMessage();
        
        HeaderCollection const& getHeaders() const;
        
        std::istream & getContent();
    };
}

#endif // SPIDER_HTTP_RESPONSE_HPP
