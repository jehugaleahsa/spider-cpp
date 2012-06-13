#include <algorithm>
#include <locale>
#include <sstream>
#include <vector>
#include <boost/array.hpp>
#include "algorithm.hpp"

#define BOOST_TEST_MODULE TestAlgorithm
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

bool isVowel(char value) {
    static array<char, 5> vowels = { 'a', 'e', 'i', 'o', 'u' };
    array<char, 5>::iterator position = find(vowels.begin(), vowels.end(), value);
    return position != vowels.end();
}

// The point of transform_while is to copy items to a destination so long as the 
// items in the source satisfy a condition. Here, we will move uppercase characters 
// to the destination as long as we encounter vowels.
BOOST_AUTO_TEST_CASE(transform_whileShouldMakeCharactersUpperCaseWhileVowels) {
    array<char, 6> values = { 'a', 'e', 'i', 'y', 'o', 'u' };
    vector<char> output;
    transform_while(
        values.begin(), values.end(),
        back_inserter(output),
        isVowel,
        ::toupper);
    BOOST_REQUIRE_MESSAGE(output.size() == 3, explain("The wrong number of items were added.", 3, output.size()));
    array<char, 3> expected = { 'A', 'E', 'I' };
    BOOST_REQUIRE_MESSAGE(equal(expected.begin(), expected.end(), output.begin()), "The wrong values were added to the output.");
}

// The algorithm should do nothing if the source is empty.
BOOST_AUTO_TEST_CASE(transform_whileShouldDoNothingIfSourceEmpty) {
    vector<char> values;
    vector<char> output;
    transform_while(
        values.begin(), values.end(),
        back_inserter(output),
        isVowel,
        ::toupper);
    BOOST_REQUIRE_MESSAGE(output.size() == 0, "Nothing should have been added to output.");
}

// If the first character fails the test, nothing should be copied.
BOOST_AUTO_TEST_CASE(transform_whileShouldDoNothingIfFirstItemFails) {
    array<char, 4> values = { 't', 'e', 's', 't' };
    vector<char> output;
    transform_while(
        values.begin(), values.end(),
        back_inserter(output),
        isVowel,
        ::toupper);
    BOOST_REQUIRE_MESSAGE(output.size() == 0, "Nothing should have been added to output.");
}

// If the last character fails the test, everything but it should be copied.
BOOST_AUTO_TEST_CASE(transform_whileShouldCopyAllButLastCharacterIfItFails) {
    array<char, 2> values = { 'a', 's' };
    vector<char> output;
    transform_while(
        values.begin(), values.end(),
        back_inserter(output),
        isVowel,
        ::toupper);
    BOOST_REQUIRE_MESSAGE(output.size() == 1, "Only the first character should have been copied.");
    array<char, 1> expected = { 'A' };
    BOOST_REQUIRE_MESSAGE(equal(expected.begin(), expected.end(), output.begin()), "The wrong values were added to the output.");
}

// If every character satisfies the predicate, they should all be copied.
BOOST_AUTO_TEST_CASE(transform_whileShouldCopyToEndIfAllItemsAreGood) {
    array<char, 3> values = { 'a', 'e', 'i' };
    vector<char> output;
    transform_while(
        values.begin(), values.end(),
        back_inserter(output),
        isVowel,
        ::toupper);
    BOOST_REQUIRE_MESSAGE(output.size() == 3, "All of the items should have been copied.");
    array<char, 3> expected = { 'A', 'E', 'I' };
    BOOST_REQUIRE_MESSAGE(equal(expected.begin(), expected.end(), output.begin()), "The wrong values were added to the output.");
}
