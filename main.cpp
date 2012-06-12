#include <iostream>
#include <cassert>
#include "download_queue.hpp"
#include "url.hpp"

using namespace spider;

int main() {
    DownloadQueue queue;
    queue.addUrl(Url::parse("http://www.google.com"));
    queue.addUrl(Url::parse("http://www.google.com"));
    queue.addUrl(Url::parse("http://www.aweber.com"));
    
    assert(queue.hasMore());
    Url firstUrl = queue.getNextUrl();
    assert(firstUrl.getHost() == "www.google.com");
    
    assert(queue.hasMore());
    Url secondUrl = queue.getNextUrl();
    assert(secondUrl.getHost() == "www.aweber.com");
    
    assert(!queue.hasMore());
    
    std::cout << "All tests ran" << std::endl;
}
