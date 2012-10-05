#include <iostream>
#include "spider.hpp"
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
    using std::cout;
    using std::string;
    using spider::Spider;
    using spider::Url;

    string topUrlString = (argc < 2) ? getStartingUrl() : argv[1];
    Url topUrl = Url::parse(topUrlString);
    Spider spider;
    spider.run(cout, topUrl);

    return 0;
}
