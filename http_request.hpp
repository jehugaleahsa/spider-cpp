#ifndef SPIDER_CPP_HTTP_REQUEST
#define SPIDER_CPP_HTTP_REQUEST

#include <string>
#include <boost/unordered_map.hpp>

namespace spider {

class HttpRequest
{
    boost::unordered_map<std::string, std::string> m_headers;

public:
    HttpRequest();
};

}

#endif // end SPIDER_CPP_HTTP_REQUEST
