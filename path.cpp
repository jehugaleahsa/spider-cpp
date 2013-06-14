#include <algorithm>
#include <string>
#include <boost/filesystem.hpp>
#include "path.hpp"

namespace {

    std::string::const_iterator getFileNamePosition(std::string const& path) {
        using std::find;
        using std::string;

        // find everything after the last slash
        string::const_reverse_iterator rposition = find(
            path.rbegin(), path.rend(), '/');
        string::const_iterator position = (rposition == path.rend())
            ? path.begin() : rposition.base();
        // distinguish between files and directories based on whether they have extensions
        string::const_iterator dotPosition = find(position, path.end(), '.');
        if (dotPosition == path.end()) {
            return path.end();
        } else {
            return position;
        }
    }

}

std::string spider::Path::getFileName(std::string const& path) {
    using std::string;

    string::const_iterator position = getFileNamePosition(path);
    string fileName(position, path.end());
    return fileName;
}

std::string spider::Path::getDirectory(std::string const& path) {
    using std::string;

    string::const_iterator position = getFileNamePosition(path);
    string directory(path.begin(), position);
    if (directory.size() == 0 || directory[0] != '/') {
        directory = '/' + directory;
    }
    return directory;
}

std::string spider::Path::getExtension(std::string const& path) {
    using std::find;
    using std::string;

    string const fileName = getFileName(path);
    string::const_reverse_iterator rposition = find(fileName.rbegin(), fileName.rend(), '.');
    if (rposition == fileName.rend()) {
        return "";
    }
    string::const_iterator position = rposition.base();
    string extension(position, fileName.end());
    return extension;
}

bool spider::Path::createDirectory(std::string const& path) {
    return boost::filesystem::create_directory(path);
}

bool spider::Path::exists(std::string const& path) {
    return boost::filesystem::exists(path);
}

bool spider::Path::isDirectory(std::string const& path) {
    return boost::filesystem::is_directory(path);
}
