#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "download_queue.hpp"
#include "extractor.hpp"
#include "file_downloader.hpp"
#include "header.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "stripper.hpp"
#include "url.hpp"

std::string getStartingUrl() {
    using std::cin;
    using std::cout;
    using std::string;

    cout << "Please enter the URL: ";
    string urlString;
    cin >> urlString;
    return urlString;
}

int main(int argc, char** argv) {
    using std::back_inserter;
    using std::for_each;
    using std::istream;
    using std::istream_iterator;
    using std::noskipws;
    using std::string;
    using std::vector;
    using boost::bind;
    using spider::DownloadQueue;
    using spider::GET;
    using spider::HttpRequest;
    using spider::Stripper;
    using spider::Url;
    using spider::UrlExtractor;

    DownloadQueue queue;
    Stripper stripper("script");

    string topUrlString = (argc < 2) ? getStartingUrl() : argv[1];
    Url topUrl = Url::parse(topUrlString);
    queue.addUrl(topUrl);

    while (queue.hasMore()) {
        Url url = queue.getNextUrl();
        std::cout << url << std::endl;

        HttpRequest request(GET, url);
        HttpRequest::response_ptr response = request.getResponse();

        istream & contentStream = response->getContent();
        contentStream >> noskipws;
        istream_iterator<char> begin(contentStream);
        istream_iterator<char> end;

        string original(begin, end);
        string stripped(stripper.strip(original));

        // Get base URL
        vector<Url> baseAddresses;
        Url baseUrl(
            url.getScheme(),
            url.getHost(),
            url.getPort(),
            url.getPath(),
            "");
        UrlExtractor baseExtractor("base", baseUrl);
        baseExtractor.getUrls(stripped, back_inserter(baseAddresses));
        if (baseAddresses.size() > 0) {
            baseUrl = baseAddresses.back();
        }

        // Get links
        vector<Url> links;
        UrlExtractor anchorExtractor("a", baseUrl);
        anchorExtractor.getUrls(stripped, back_inserter(links));
        for_each(
            links.begin(), links.end(),
            bind(&DownloadQueue::addUrl, &queue, _1));
    }
}
