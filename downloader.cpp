#include "counter.hpp"
#include "downloader.hpp"
#include "http_request.hpp"
#include "url.hpp"

namespace spider {

    void Downloader::addReferrerHeader(spider::HttpRequest & request) const {
        using std::ostringstream;
        using spider::HeaderCollection;

        HeaderCollection & headers = request.getHeaders();
        ostringstream referrerBuilder;
        referrerBuilder << m_referrer;
        headers.addHeader("referer", referrerBuilder.str());
    }

    void Downloader::addUserAgentHeader(spider::HttpRequest & request) const {
        using spider::HeaderCollection;

        HeaderCollection & headers = request.getHeaders();
        headers.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/22.0.1207.1 Safari/537.1");
    }

    void Downloader::addAcceptHeader(spider::HttpRequest & request) const {
        using spider::HeaderCollection;

        HeaderCollection & headers = request.getHeaders();
        headers.addHeader("Accept", "*/*");
    }

    void Downloader::addHostHeader(spider::HttpRequest & request) const {
        using spider::HeaderCollection;

        HeaderCollection & headers = request.getHeaders();
        headers.addHeader("Host", m_url.getHost());
    }

    void Downloader::addConnectionHeader(spider::HttpRequest & request) const {
        using spider::HeaderCollection;

        HeaderCollection & headers = request.getHeaders();
        headers.addHeader("Connection", "close");
    }

    Downloader::Downloader(
        Counter & counter,
        Url const& url,
        Url const& referrer)
        : m_counter(counter), m_url(url), m_referrer(referrer) {
        m_counter.increment();
    }

    Counter & Downloader::getCounter() {
        return m_counter;
    }

    Url const& Downloader::getUrl() const {
        return m_url;
    }

    Url const& Downloader::getReferrer() const {
        return m_referrer;
    }

    Downloader::~Downloader() {
        m_counter.decrement();
    }
}
