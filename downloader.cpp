#include <sstream>
#include "counter.hpp"
#include "downloader.hpp"
#include "http_request.hpp"
#include "url.hpp"

void spider::Downloader::addReferrerHeader(spider::HttpRequest & request) const {
    using std::ostringstream;
    using spider::HeaderCollection;

    if (!m_referrer) {
        return;
    }
    HeaderCollection & headers = request.getHeaders();
    ostringstream referrerBuilder;
    referrerBuilder << *m_referrer;
    headers.addHeader("referer", referrerBuilder.str());
}

void spider::Downloader::addUserAgentHeader(spider::HttpRequest & request) const {
    using spider::HeaderCollection;

HeaderCollection & headers = request.getHeaders();
    //headers.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/22.0.1207.1 Safari/537.1");
    headers.addHeader("User-Agent", "curl/7.22.0 (x86_64-pc-linux-gnu) libcurl/7.22.0 OpenSSL/1.0.1 zlib/1.2.3.4 libidn/1.23 librtmp/2.3");
}

void spider::Downloader::addAcceptHeader(spider::HttpRequest & request) const {
    using spider::HeaderCollection;
    
    HeaderCollection & headers = request.getHeaders();
    headers.addHeader("Accept", "*/*");
}

void spider::Downloader::addHostHeader(spider::HttpRequest & request) const {
    using spider::HeaderCollection;

    HeaderCollection & headers = request.getHeaders();
    headers.addHeader("Host", m_url.getHost());
}

void spider::Downloader::addConnectionHeader(spider::HttpRequest & request) const {
    using spider::HeaderCollection;

    HeaderCollection & headers = request.getHeaders();
    headers.addHeader("Connection", "close");
}

spider::Downloader::Downloader(
    Counter & counter,
    Url const& url,
    std::shared_ptr<Url> const referrer)
    : m_counter(counter), m_url(url), m_referrer(referrer) {
}

spider::Downloader::~Downloader() {
}

spider::Counter & spider::Downloader::getCounter() {
    return m_counter;
}

spider::Url const& spider::Downloader::getUrl() const {
    return m_url;
}

std::shared_ptr<spider::Url> const spider::Downloader::getReferrer() const {
    return m_referrer;
}
