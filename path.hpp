#ifndef SPIDER_PATH_HPP
#define SPIDER_PATH_HPP

#include <string>

namespace spider {

    class Path {
        Path();
        Path(Path const& other);
        Path & operator=(Path const& other);

    public:
        static std::string getFileName(std::string const& path);
    
        static std::string getDirectory(std::string const& path);
    
        static std::string getExtension(std::string const& path);

        static bool createDirectory(std::string const& path);

        static bool exists(std::string const& path);

        static bool isDirectory(std::string const& path);

        static uintmax_t size(std::string const& path);

        static void remove(std::string const& path);

    };

}

#endif // SPIDER_PATH_HPP
