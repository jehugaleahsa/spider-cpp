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

        mutable std::shared_ptr<std::istream> m_stream;

        mutable bool m_hasStatus;
        mutable std::string m_version;
        mutable int m_statusCode;
        mutable std::string m_statusMessage;
        void getStatusCached() const;

        mutable bool m_hasHeaders;
        mutable HeaderCollection m_headers;
        void getHeadersCached() const;

        HttpResponse(std::shared_ptr<std::istream> stream);

        bool readLine(std::string & line);

    public:
        HttpResponse(HttpResponse const& other);

        std::string getVersion() const;

        int getStatusCode() const;

        std::string getStatusMessage() const;

        HeaderCollection const& getHeaders() const;

        std::istream & getContent();
    };
}

#endif // SPIDER_HTTP_RESPONSE_HPP
