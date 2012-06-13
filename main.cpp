#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include "file_downloader.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "url.hpp"

using namespace std;
using namespace spider;

class HeaderPrinter : unary_function<void, string> {
    HttpResponse & m_response;
    
public:
    HeaderPrinter(HttpResponse & response)
        : m_response(response) {
    }
    
    void operator ()(string const& name) {
        vector<string> values;
        m_response.getHeaderValues(name, back_inserter(values));
        cerr << name << ": ";
        ostream_iterator<string> destination(cerr, ", ");
        copy(values.begin(), values.end(), destination);
        cerr << endl;
    }
};

int main() {
    Url url = Url::parse("http://www.google.com/");
    HttpRequest request(GET, url);
    HttpResponse response = request.getResponse();
    cerr << response.getStatusCode() << endl;
    vector<string> names;
    response.getHeaderNames(back_inserter(names));
    for_each(names.begin(), names.end(), HeaderPrinter(response));
    istream_iterator<char> begin(response.getContent() >> noskipws);
    istream_iterator<char> end;
    ostream_iterator<char> destination(cerr);
    copy(begin, end, destination);
}
