#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <queue>
#include <boost/shared_ptr.hpp>
#include "file_downloader.hpp"
#include "header.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
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
    using std::istream;
    using std::queue;
    using std::string;
    using spider::GET;
    using spider::HttpRequest;
    using spider::Url;

    queue<Url> urls;

    string topUrlString;
    if (argc < 2) {
        topUrlString = getStartingUrl();
    } else {
        topUrlString = argv[1];
    }
    Url topUrl = Url::parse(topUrlString);
    urls.push(topUrl);

    while (!urls.empty()) {
        Url url = urls.front();
        urls.pop();
        HttpRequest request(GET, url);

        HttpRequest::response_ptr response = request.getResponse();
        if (response->getStatusCode() == 200) {
            istream & contentStream = response->getContent();
        }
    }
}
