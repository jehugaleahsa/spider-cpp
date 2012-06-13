#include <sstream>
#include <string>
#include "path_utilities.hpp"

#define BOOST_TEST_MODULE TestPathUtilities
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

// If the path has a file name with an extension, that file name should be returned.
BOOST_AUTO_TEST_CASE(getFileNameShouldGetNameOfFile) {
    string path = "/path/to/file.html";
    string fileName = getFileName(path);
    string expected = "file.html";
    BOOST_REQUIRE_MESSAGE(fileName == expected, explain("The wrong file name was extracted.", expected, fileName));
}

// If the path has a file name with an extension, everything before it should be returned.
BOOST_AUTO_TEST_CASE(getDirectoryShouldRemoveFileName) {
    string path = "/path/to/file.html";
    string directory = getDirectory(path);
    string expected = "/path/to/";
    BOOST_REQUIRE_MESSAGE(directory == expected, explain("The wrong directory was extracted.", expected, directory));
}

// There is no way to determine whether the last part of a path is a file or a directory.
// We'll ignore this problem by treating anything with an extension as a file.
BOOST_AUTO_TEST_CASE(getFileNameShouldReturnNothingIfThereIsNoExtension) {
    string path = "/path/to/directory";
    string fileName = getFileName(path);
    string expected = "";
    BOOST_REQUIRE_MESSAGE(fileName == expected, explain("The wrong file name was extracted.", expected, fileName)); 
}

// There is no way to determine whether the last part of a path is a file or a directory.
// We'll ignore this problem by treating anything with an extension as a file.
BOOST_AUTO_TEST_CASE(getDirectoryShouldReturnTheOriginalIfThereIsNoExtension) {
    string path = "/path/to/directory";
    string directory = getDirectory(path);
    BOOST_REQUIRE_MESSAGE(directory == path, explain("The wrong directory was extracted.", path, directory)); 
}

// If the last character in the path is a slash, then there is no file name.
BOOST_AUTO_TEST_CASE(getFileNameShouldReturnNothingIfPathEndsInSlash) {
    string path = "/path/to/directory/";
    string fileName = getFileName(path);
    string expected = "";
    BOOST_REQUIRE_MESSAGE(fileName == expected, explain("The wrong file name was extracted.", expected, fileName));
}

// If the last character in the path is a slash, then the entire path is the directory.
BOOST_AUTO_TEST_CASE(getDirectoryShouldReturnOriginalIfPathEndsInSlash) {
    string path = "/path/to/directory/";
    string directory = getDirectory(path);
    BOOST_REQUIRE_MESSAGE(directory == path, explain("The wrong directory was extracted.", path, directory));
}

// If the file name is the only part of the path, the whole path should be returned.
BOOST_AUTO_TEST_CASE(getFileNameShouldReturnOriginalStringIfJustFileName) {
    string path = "file.html";
    string fileName = getFileName(path);
    BOOST_REQUIRE_MESSAGE(fileName == path, explain("The wrong file name was extracted.", path, fileName));
}

// If the file name is the only part of the path, then the directory should be a single slash.
BOOST_AUTO_TEST_CASE(getDirectoryShouldDefaultToSlashIfJustFileName) {
    string path = "file.html";
    string directory = getDirectory(path);
    string expected = "/";
    BOOST_REQUIRE_MESSAGE(directory == expected, explain("The wrong directory was extracted.", expected, directory));
}

// If the path is the file name following a single slash, the slash should be removed.
BOOST_AUTO_TEST_CASE(getFileNameShouldDropLeadingSlash) {
    string path = "/file.html";
    string fileName = getFileName(path);
    string expected = "file.html";
    BOOST_REQUIRE_MESSAGE(fileName == expected, explain("The wrong file name was extracted.", expected, fileName));
}

// If the path is the file name following a single slash, the slash should be returned.
BOOST_AUTO_TEST_CASE(getDirectoryShouldReturnLeadingSlash) {
    string path = "/file.html";
    string directory = getDirectory(path);
    string expected = "/";
    BOOST_REQUIRE_MESSAGE(directory == expected, explain("The wrong directory was extracted.", expected, directory));
}

// If the path is missing its leading slash, we should add it.
BOOST_AUTO_TEST_CASE(getDirectoryShouldAddLeadingSlash) {
    string path = "path/to/file.html";
    string directory = getDirectory(path);
    string expected = "/path/to/";
    BOOST_REQUIRE_MESSAGE(directory == expected, explain("The wrong directory was extracted.", expected, directory));
}

// If we try to get the extension of the file in a path, it should be returned.
BOOST_AUTO_TEST_CASE(getExtensionShouldGetExtension) {
    string path = "/path/to/file.html";
    string extension = getExtension(path);
    string expected = "html";
    BOOST_REQUIRE_MESSAGE(extension == expected, explain("The wrong extension was extracted.", expected, extension));
}

// If we try to get the extension of the file in a path without a file name, nothing should be returned.
BOOST_AUTO_TEST_CASE(getExtensionShouldReturnNothingIfFileNotProvided) {
    string path = "/path/to/";
    string extension = getExtension(path);
    string expected = "";
    BOOST_REQUIRE_MESSAGE(extension == expected, explain("The wrong extension was extracted.", expected, extension));
}

// If the file name has multiple extensions, only the extension after the last dot should be returned.
BOOST_AUTO_TEST_CASE(getExtensionShouldReturnLastExtensionIfMultiple) {
    string path = "/path/to/file.with.three.exts";
    string extension = getExtension(path);
    string expected = "exts";
    BOOST_REQUIRE_MESSAGE(extension == expected, explain("The wrong extension was extracted.", expected, extension));
}
