#ifndef SPIDER_HTTP_RESPONSE_HPP
#define SPIDER_HTTP_RESPONSE_HPP

#include <iostream>
#include <set>
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

        boost::shared_ptr<boost::asio::ip::tcp::iostream> m_stream;

        bool m_isInitialized;

        bool m_hasStatus;
        void getStatusCached();
        int m_status;

        bool m_hasHeaders;
        void getHeadersCached();
        header_collection_type m_headers;

        HttpResponse(boost::shared_ptr<boost::asio::ip::tcp::iostream> stream);

        bool readLine(std::string & line);

    public:
        int getStatus() const;

        template <typename TOutIterator>
        void getHeaderNames(TOutIterator destination);

        template <typename TOutIterator>
        void getHeaderValues(std::string const& name, TOutIterator destination);
        
        std::istream & getContent();
    };
    
    namespace {
        std::string getHeaderValue(HttpResponse::header_collection_type::value_type const& pair) {
            return pair.second;
        }
        
        std::string getHeaderName(HttpResponse::header_collection_type::value_type const& pair) {
            return pair.first;
        }
    }

    template <typename TOutIterator>
    void HttpResponse::getHeaderValues(std::string const& name, TOutIterator destination) {
        using std::pair;
        using std::transform;
        using boost::unordered_multimap;

        const_cast<HttpResponse&>(*this).getHeadersCached();
        typedef header_collection_type::const_iterator iterator;
        pair<iterator, iterator> range = m_headers.equal_range(name);
        transform(range.first, range.second, destination, getHeaderValue);
    }

    template <typename TOutIterator>
    void HttpResponse::getHeaderNames(TOutIterator destination) {
        using std::inserter;
        using std::copy;
        using std::set;
        using std::string;
        using std::transform;

        const_cast<HttpResponse&>(*this).getHeadersCached();
        set<string> names;
        transform(m_headers.begin(), m_headers.end(), inserter(names, names.end()), getHeaderName);
        copy(names.begin(), names.end(), destination);
    }
}

#endif // SPIDER_HTTP_RESPONSE_HPP
