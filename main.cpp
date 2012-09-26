#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "download_queue.hpp"
#include "file_downloader.hpp"
#include "header.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "extractor.hpp"
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
    using std::string;
    using std::vector;
    using boost::bind;
    using spider::DownloadQueue;
    using spider::GET;
    using spider::HttpRequest;
    using spider::Url;
    using spider::UrlExtractor;

    DownloadQueue queue;

    string topUrlString = (argc < 2) ? getStartingUrl() : argv[1];
    Url topUrl = Url::parse(topUrlString);
    queue.addUrl(topUrl);

    while (queue.hasMore()) {
        Url url = queue.getNextUrl();
        std::cout << "Downloading " << url << std::endl;

        HttpRequest request(GET, url);
        HttpRequest::response_ptr response = request.getResponse();
        if (response->getStatusCode() == 200) {
            istream & contentStream = response->getContent();
            istream_iterator<char> begin(contentStream);
            istream_iterator<char> end;
            string content(begin, end);
            UrlExtractor extractor("a", url);
            vector<Url> linked;
            extractor.getUrls(content, back_inserter(linked));
            for_each(
                linked.begin(), linked.end(),
                bind(&DownloadQueue::addUrl, &queue, _1));
        }
    }
}
