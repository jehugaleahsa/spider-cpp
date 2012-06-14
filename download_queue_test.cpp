#include <sstream>
#include "download_queue.hpp"

#define BOOST_TEST_MODULE TestDownloadQueue
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;
using namespace spider;

template <typename TExpected, typename TActual>
string explain(string const& message, TExpected const& expected, TActual const& actual) {
    ostringstream builder(message);
    builder << " Expected: " << expected << " Actual: " << actual << std::endl;
    return builder.str();
}

// If we are creating a new queue, it should be empty.
BOOST_AUTO_TEST_CASE(constructorShouldCreateEmptyQueue) {
    DownloadQueue queue;
    BOOST_REQUIRE_MESSAGE(!queue.hasMore(), "An empty queue should not have any items.");
}

// Items should be removed in the same order they're added (FIFO).
BOOST_AUTO_TEST_CASE(getNextUrlShouldGetTheFirstItemAdded) {
    DownloadQueue queue;
    queue.addUrl(Url::parse("http://www.google.com"));
    queue.addUrl(Url::parse("http://www.truncon.com"));

    BOOST_REQUIRE_MESSAGE(queue.hasMore(), "The queue should not have had two URLs remaining.");
    Url first = queue.getNextUrl();
    const string firstExpectedHost = "www.google.com";
    BOOST_REQUIRE_MESSAGE(first.getHost() == firstExpectedHost, explain("The first URL was wrong.", firstExpectedHost, first.getHost()));
    
    BOOST_REQUIRE_MESSAGE(queue.hasMore(), "The queue should not have had one URL remaining.");
    Url second = queue.getNextUrl();
    const string secondExpectedHost = "www.truncon.com";
    BOOST_REQUIRE_MESSAGE(second.getHost() == secondExpectedHost, explain("The second URL was wrong.", secondExpectedHost, second.getHost()));
}

// The same item should not be added twice.
BOOST_AUTO_TEST_CASE(addUrlShouldNotAddTheSameUrlTwice) {
    DownloadQueue queue;
    
    Url url = Url::parse("http://www.google.com");
    queue.addUrl(url);
    queue.addUrl(url);

    BOOST_REQUIRE_MESSAGE(queue.hasMore(), "The queue should not have been empty.");
    Url actual = queue.getNextUrl();
    const string expectedHost = "www.google.com";
    BOOST_REQUIRE_MESSAGE(actual.getHost() == expectedHost, explain("The URL was wrong.", expectedHost, actual.getHost()));
    
    BOOST_REQUIRE_MESSAGE(!queue.hasMore(), "The queue should have been empty.");
}

// The same item should not be added twice, even if the first occurrence has already been removed.
BOOST_AUTO_TEST_CASE(addUrlShouldNotAddTheSameUrlTwiceEvenIfRemoved) {
    DownloadQueue queue;
    
    Url url = Url::parse("http://www.google.com");
    queue.addUrl(url);
    BOOST_REQUIRE_MESSAGE(queue.hasMore(), "The queue should not have been empty.");
    Url actual = queue.getNextUrl();
    const string expectedHost = "www.google.com";
    BOOST_REQUIRE_MESSAGE(actual.getHost() == expectedHost, explain("The URL was wrong.", expectedHost, actual.getHost()));
    
    queue.addUrl(url); // add it after it's been already queued
    BOOST_REQUIRE_MESSAGE(!queue.hasMore(), "The queue should have been empty.");
}
