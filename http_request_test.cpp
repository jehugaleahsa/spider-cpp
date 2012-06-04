#include <algorithm>
#include <iostream>
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

template <typename T>
string explain(string const& message, T const& expected, T const& actual) {
    ostringstream builder(message);
    builder << " Expected: " << expected << " Actual: " << actual << std::endl;
    return builder.str();
}

class HeaderPrinter {
    HttpResponse const& m_response;

    public:
    HeaderPrinter(HttpResponse const& response)
        : m_response(response) {
    }

    string operator ()(string const& headerName) {
        vector<string> values;
        m_response.getHeaderValues(headerName, values);
        ostringstream builder;
        builder << headerName << ": ";
        copy(values.begin(), values.end(), ostream_iterator<string>(builder, ", "));
        return builder.str();
    }
};

BOOST_AUTO_TEST_CASE(shouldCreateDefaultRequest) {
    Url url = Url::parse("http://www.google.com/");
    HttpRequest request(GET, url);
    HttpResponse response = request.getResponse();
    int statusCode = response.getStatus();
    BOOST_REQUIRE_MESSAGE(statusCode == 200, "Google did not return an OK status code.");
    vector<string> headerNames;
    response.getHeaderNames(headerNames);
    transform(headerNames.begin(), headerNames.end(), ostream_iterator<string>(cerr, "\n"), HeaderPrinter(response));
}
