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

        template <typename TOutIterator>
        void getHeaderNames(TOutIterator destination) const;

        template <typename TOutIterator>
        void getHeaderValues(std::string const& name, TOutIterator destination) const;
        
        template <typename TOutIterator, typename TChar>
        bool getNextContentChunk(TOutIterator destination);
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
    void HttpResponse::getHeaderValues(std::string const& name, TOutIterator destination) const {
        using std::pair;
        using std::transform;
        using boost::unordered_multimap;

        const_cast<HttpResponse&>(*this).getHeadersCached();
        typedef header_collection_type::const_iterator iterator;
        pair<iterator, iterator> range = m_headers.equal_range(name);
        transform(range.first, range.second, destination, getHeaderValue);
    }

    template <typename TOutIterator>
    void HttpResponse::getHeaderNames(TOutIterator destination) const {
        using std::back_inserter;
        using std::copy;
        using std::sort;
        using std::string;
        using std::transform;
        using std::unique;
        using std::vector;

        const_cast<HttpResponse&>(*this).getHeadersCached();
        vector<string> names;
        transform(m_headers.begin(), m_headers.end(), back_inserter(names), getHeaderName);
        sort(names.begin(), names.end());
        vector<string>::iterator position = unique(names.begin(), names.end());
        names.erase(position, names.end());
        copy(names.begin(), names.end(), destination);
    }
    
    template <typename TOutIterator, typename TChar>
    bool HttpResponse::getNextContentChunk(TOutIterator destination) {
        using std::copy;
        using std::istream;
        using std::istream_iterator;
        using std::noskipws;
        using boost::asio::error::eof;
        using boost::asio::read;
        using boost::asio::transfer_at_least;
        using boost::system::error_code;
        using boost::system::system_error;
    
        getHeadersCached();
        
        error_code error;
        read(*m_socket, *m_buffer, transfer_at_least(1), error);
        bool hasMore = true;
        if (error) {
            if (error == eof) {
                hasMore = false;
            } else {
                throw system_error(error);
            }
        }
        
        istream reader(m_buffer.get());
        istream_iterator<TChar> position(reader >> noskipws);
        istream_iterator<TChar> end;
        copy(position, end, destination);
        return hasMore;
    }
}

#endif // SPIDER_HTTP_RESPONSE_HPP
