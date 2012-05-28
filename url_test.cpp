#include <iostream>
#include "url.hpp"

#define BOOST_TEST_MODULE TestUrl
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace spider;

BOOST_AUTO_TEST_CASE(parseShouldParseHost) {
    Url url = Url::parse("www.google.com");
    BOOST_REQUIRE_MESSAGE(url.getScheme() == Url::getHttpScheme(), "The default scheme was not used.");
    BOOST_REQUIRE_MESSAGE(url.getHost() == "www.google.com", "The host name was not parsed.");
    BOOST_REQUIRE_MESSAGE(url.getPort() == Url::getDefaultPort(), "The default port was not used.");
    BOOST_REQUIRE_MESSAGE(url.getPath() == "", "The path was not empty.");
    BOOST_REQUIRE_MESSAGE(url.getQuery() == "", "The query was not empty.");
}

BOOST_AUTO_TEST_CASE(parseShouldParseScheme) {
    Url url = Url::parse("https://www.google.com");
    BOOST_REQUIRE_MESSAGE(url.getScheme() == Url::getHttpsScheme(), "The scheme was not extracted.");
    BOOST_REQUIRE_MESSAGE(url.getHost() == "www.google.com", "The host name was not parsed.");
    BOOST_REQUIRE_MESSAGE(url.getPort() == Url::getDefaultPort(), "The default port was not used.");
    BOOST_REQUIRE_MESSAGE(url.getPath() == "", "The path should have been empty.");
    BOOST_REQUIRE_MESSAGE(url.getQuery() == "", "The query should have been empty.");
}
