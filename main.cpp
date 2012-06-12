#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include "http_request.hpp"
#include "http_response.hpp"

using namespace std;
using namespace spider;

int main() {
    Url url = Url::parse("http://www.google.com/");
    HttpRequest request(GET, url);
    HttpResponse response = request.getResponse();
    istream_iterator<char> begin(response.getContent());
    istream_iterator<char> end;
    ostream_iterator<char> destination(cerr);
    copy(begin, end, destination);
    std::cerr << std::endl;
}
