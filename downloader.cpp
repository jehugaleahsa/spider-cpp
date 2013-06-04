#include <sstream>
#include "downloader.hpp"
#include "http_request.hpp"
#include "url.hpp"

void spider::Downloader::addReferrerHeader(spider::HttpRequest & request) const {
    using std::ostringstream;
    using spider::HeaderCollection;

    if (m_referrer.empty()) {
        return;
    }
    HeaderCollection & headers = request.getHeaders();
    ostringstream referrerBuilder;
    referrerBuilder << m_referrer;
    headers.addHeader("referer", referrerBuilder.str());
}

void spider::Downloader::addUserAgentHeader(spider::HttpRequest & request) const {
    using spider::HeaderCollection;

HeaderCollection & headers = request.getHeaders();
    headers.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/22.0.1207.1 Safari/537.1");
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

spider::Downloader::Downloader(Url const& url, Url const& referrer)
    : m_url(url), m_referrer(referrer) {
}

spider::Downloader::~Downloader() {
}

spider::Url const& spider::Downloader::getUrl() const {
    return m_url;
}

spider::Url const& spider::Downloader::getReferrer() const {
    return m_referrer;
}
