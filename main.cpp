#include "file_downloader.hpp"
#include "url.hpp"

using namespace spider;

int main() {
    FileDownloader downloader("/home/travis/spider-test/");
    Url url = Url::parse("http://www.google.com/images/srpr/logo3w.png");
    downloader.download(url);
}
