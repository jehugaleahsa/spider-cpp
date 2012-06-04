#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include "url.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

#define BOOST_TEST_MODULE TestHttpRequest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace spider;

BOOST_AUTO_TEST_CASE(shouldCreateDefaultRequest) {
    Url url = Url::parse("http://www.google.com/");
    HttpRequest request(GET, url);
    HttpResponse response = request.getResponse();
    int statusCode = response.getStatus();
    BOOST_REQUIRE_MESSAGE(statusCode == 200, "Google did not return an OK status code.");
    vector<string> headerNames;
    response.getHeaderNames(back_inserter(headerNames));
    ostream_iterator<char> output(cout, "");
    while (response.getNextContentChunk(output)) {
    }
}
