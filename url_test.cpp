#include <iostream>
#include <sstream>
#include "url.hpp"
#include "bad_url_exception.hpp"

#define BOOST_TEST_MODULE TestUrl
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

void checkParsedValues(
    Url const& url,
    string const& scheme,
    string const& host,
    int port,
    string const& path,
    string const& query) {
    BOOST_REQUIRE_MESSAGE(url.getScheme() == scheme, explain("The scheme was wrong.", scheme, url.getScheme()));
    BOOST_REQUIRE_MESSAGE(url.getHost() == host, explain("The host name was wrong.", host, url.getHost()));
    BOOST_REQUIRE_MESSAGE(url.getPort() == port, explain("The port was wrong.", port, url.getPort()));
    BOOST_REQUIRE_MESSAGE(url.getPath() == path, explain("The path was wrong.", path, url.getPath()));
    BOOST_REQUIRE_MESSAGE(url.getQuery() == query, explain("The query was wrong.", query, url.getQuery()));
}

BOOST_AUTO_TEST_CASE(parseShouldParseHost) {
    Url url = Url::parse("www.google.com");
    checkParsedValues(url, Url::getHttpScheme(), "www.google.com", Url::getDefaultPort(), "", "");
}

BOOST_AUTO_TEST_CASE(parseShouldParseScheme) {
    Url url = Url::parse("https://www.google.com");
    checkParsedValues(url, "https", "www.google.com", Url::getDefaultPort(), "", "");
}

BOOST_AUTO_TEST_CASE(parseShouldParsePort) {
    Url url = Url::parse("http://www.google.com:8080");
    checkParsedValues(url, "http", "www.google.com", 8080, "", "");
}

BOOST_AUTO_TEST_CASE(parseShouldParsePath) {
    Url url = Url::parse("http://www.google.com/path/to/page");
    checkParsedValues(url, "http", "www.google.com", Url::getDefaultPort(), "/path/to/page", "");
}

BOOST_AUTO_TEST_CASE(parseShouldParsePortAndPath) {
    Url url = Url::parse("http://www.google.com:8080/");
    checkParsedValues(url, "http", "www.google.com", 8080, "/", "");
}

BOOST_AUTO_TEST_CASE(parseShouldParseQuery) {
    Url url = Url::parse("www.google.com?param=123");
    checkParsedValues(url, Url::getHttpScheme(), "www.google.com", Url::getDefaultPort(), "/", "param=123");
}

BOOST_AUTO_TEST_CASE(parseShouldThrowIfBadUrl) {
    // I am assuming that a colon (:) followed by a non-integer will be bad
    BOOST_REQUIRE_THROW(Url::parse("bad_url:abc"), BadUrlException);
}
