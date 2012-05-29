#include <iostream>
#include <sstream>
#include "url.hpp"
#include "http_request.hpp"

#define BOOST_TEST_MODULE TestHttpRequest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace spider;

template <typename T>
string explain(string const& message, T const& expected, T const& actual) {
    ostringstream builder(message);
    builder << " Expected: " << expected << " Actual: " << actual << std::endl;
    return builder.str();
}

BOOST_AUTO_TEST_CASE(shouldCreateDefaultRequest) {
    Url url = Url::parse("http://www.google.com");
    HttpRequest request(GET, url);
}
