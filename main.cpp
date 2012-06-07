#include <iostream>
#include <string>
#include "page_downloader.hpp"
#include "file_downloader.hpp"

using namespace std;
using namespace spider;

int main() {
    try {
        Url url = Url::parse("http://www.google.com/");
        PageDownloader downloader;
        string content = downloader.download(url);
        cout << content << endl;
        
        Url fileUrl = Url::parse("http://www.google.com/images/logos/google_logo_41.png");
        FileDownloader fileDownloader("/home/travis/spider-test");
        fileDownloader.download(fileUrl);
    } catch (...) {
        std::cerr << "An Error Occurred." << std::endl;
    }
}
