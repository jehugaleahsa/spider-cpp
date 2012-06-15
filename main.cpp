#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <boost/shared_ptr.hpp>
#include "file_downloader.hpp"
#include "header.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "url.hpp"

using namespace std;
using namespace boost;
using namespace spider;

void printHeader(Header const& header) {
    cerr << header.getName() << ": ";
    ostream_iterator<string> output(cerr, ", ");
    copy(header.begin(), header.end(), output);
    cerr << endl;
}

int main() {
    Url url = Url::parse("http://www.google.com/");
    HttpRequest request(GET, url);
    request.getHeaders().addHeader("referer", "me like tacos");
    
    HttpRequest::response_ptr response = request.getResponse();
    cerr << response->getStatusCode() << endl;
    
    istream_iterator<char> begin(response->getContent() >> noskipws);
    istream_iterator<char> end;
    ostream_iterator<char> destination(cerr);
    copy(begin, end, destination);
    cerr << endl;
    
    HeaderCollection const& headers = response->getHeaders();
    for_each(headers.begin(), headers.end(), printHeader);
}
