#include <iostream>
#include <string>
#include "page_downloader.hpp"

using namespace std;
using namespace spider;

int main() {
    Url url = Url::parse("http://www.google.com/");
    PageDownloader downloader;
    string content = downloader.download(url);
    cout << content << endl;
}
