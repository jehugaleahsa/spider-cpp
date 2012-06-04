#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "url.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

using namespace std;
using namespace spider;

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

int main() {
    Url url = Url::parse("http://www.google.com/");
    HttpRequest request(GET, url);
    HttpResponse response = request.getResponse();
    int statusCode = response.getStatus();
    cerr << "Status Code: " << statusCode << endl;
    vector<string> headerNames;
    response.getHeaderNames(headerNames);
    transform(headerNames.begin(), headerNames.end(), ostream_iterator<string>(cerr, "\n"), HeaderPrinter(response));
    cerr << "Done" << endl;
}
