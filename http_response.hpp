#ifndef SPIDER_HTTP_RESPONSE_HPP
#define SPIDER_HTTP_RESPONSE_HPP

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

namespace spider {
    class HttpResponse {
    public:
        typedef boost::unordered_multimap<std::string, std::string> header_collection_type;

    private:
        friend class HttpRequest;

        boost::shared_ptr<boost::asio::io_service> m_service;
        boost::shared_ptr<boost::asio::ip::tcp::socket> m_socket;
        boost::shared_ptr<boost::asio::streambuf> m_buffer;

        bool m_isInitialized;

        bool m_hasStatus;
        void getStatusCached();
        int m_status;

        bool m_hasHeaders;
        void getHeadersCached();
        header_collection_type m_headers;

        HttpResponse(
            boost::shared_ptr<boost::asio::io_service> service,
            boost::shared_ptr<boost::asio::ip::tcp::socket> socket);

        bool readLine(std::string & line);

    public:
        int getStatus() const;

        void getHeaderNames(std::vector<std::string> & names) const;

        void getHeaderValues(std::string const& name, std::vector<std::string> & values) const;
    };
}

#endif // SPIDER_HTTP_RESPONSE_HPP
