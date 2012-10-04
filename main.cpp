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

class Printer {
    int m_value;

public:
    Printer(int value) : m_value(value) {
    }

    void operator()() {
        std::cout << m_value << std::endl;
    }
};

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
