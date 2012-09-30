#ifndef SPIDER_PATH_UTILITIES_HPP
#define SPIDER_PATH_UTILITIES_HPP

#include <algorithm>
#include <string>

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

namespace spider {

    std::string getFileName(std::string const& path) {
        using std::string;

        string::const_iterator position = getFileNamePosition(path);
        string fileName(position, path.end());
        return fileName;
    }

    std::string getDirectory(std::string const& path) {
        using std::string;

        string::const_iterator position = getFileNamePosition(path);
        string directory(path.begin(), position);
        if (directory.size() == 0 || directory[0] != '/') {
            directory = '/' + directory;
        }
        return directory;
    }

    std::string getExtension(std::string const& path) {
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

}

#endif // SPIDER_PATH_UTILITIES_HPP
