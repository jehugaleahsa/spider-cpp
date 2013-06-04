#ifndef SPIDER_HTTP_RESPONSE_HPP
#define SPIDER_HTTP_RESPONSE_HPP

#include <iosfwd>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include "header.hpp"

namespace spider {

    class HttpResponse {
        friend class HttpRequest;

        std::shared_ptr<std::istream> m_stream;

        bool m_hasStatus;
        void getStatusCached();
        std::string m_version;
        int m_statusCode;
        std::string m_statusMessage;

        bool m_hasHeaders;
        void getHeadersCached();
        HeaderCollection m_headers;

        HttpResponse(std::shared_ptr<std::istream> stream);

        bool readLine(std::string & line);

    public:
        HttpResponse(HttpResponse const& other);

        std::string getVersion();

        int getStatusCode();

        std::string getStatusMessage();

        HeaderCollection const& getHeaders() const;

        std::istream & getContent();
    };
}

#endif // SPIDER_HTTP_RESPONSE_HPP
