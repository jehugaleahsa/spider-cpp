#include "downloader.hpp"
#include "http_request.hpp"
#include "url.hpp"

namespace spider {

    void Downloader::addReferrerHeader(
        spider::HttpRequest & request, spider::Url const& url) {
        using std::ostringstream;
        using spider::HeaderCollection;

        HeaderCollection & headers = request.getHeaders();
        ostringstream referrerBuilder;
        referrerBuilder << url;
        headers.addHeader("referer", referrerBuilder.str());
    }

    void Downloader::addUserAgentHeader(spider::HttpRequest & request) {
        using spider::HeaderCollection;

        HeaderCollection & headers = request.getHeaders();
        headers.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/22.0.1207.1 Safari/537.1");
    }

    void Downloader::addAcceptHeader(spider::HttpRequest & request) {
        using spider::HeaderCollection;

        HeaderCollection & headers = request.getHeaders();
        headers.addHeader("Accept", "*/*");
    }

    void Downloader::addHostHeader(spider::HttpRequest & request, spider::Url const& url) {
        using spider::HeaderCollection;

        HeaderCollection & headers = request.getHeaders();
        headers.addHeader("Host", url.getHost());
    }

    void Downloader::addConnectionHeader(spider::HttpRequest & request) {
        using spider::HeaderCollection;

        HeaderCollection & headers = request.getHeaders();
        headers.addHeader("Connection", "close");
    }

    Downloader::Downloader() {
    }

    Downloader::~Downloader() {
    }

}
