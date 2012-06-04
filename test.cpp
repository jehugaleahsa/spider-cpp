#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::system;

    bool readLine(ip::tcp::socket & socket, boost::asio::streambuf & buffer, string & line) {
        // NOTE: There may be lines in the buffer from previous runs.
        // We need to first try grabbing a line, then check for stream errors.
        // If the stream ran out of characters, we need to read more from
        // the socket. From there, we can grab the next line.
        istream reader(&buffer);
        getline(reader, line);
        if (reader.eof()) {
            reader.clear();
            error_code error;
            size_t bytesRead = read_until(socket, buffer, "\r\n", error);
            if (error != 0) {
                if (error == boost::asio::error::eof) {
                    return false;
                } else {
                    throw boost::system::system_error(error);
                }
            }
            string remaining;
            getline(reader, remaining);
            line += remaining;
        }
        if (line.size() > 0) {
            line.erase(--line.end()); // chop off trailing \r
        }
        return true;
    }

boost::asio::ip::tcp::socket * getSocket() {
    using std::string;
    using std::ostream;
    using boost::shared_ptr;
    using boost::unordered_map;
    using boost::asio::ip::tcp;
    using boost::asio::io_service;

    tcp::resolver::query nameQuery("www.google.com", "http");

    io_service service;
    tcp::resolver resolver(service);
    tcp::resolver::iterator iterator = resolver.resolve(nameQuery);

    boost::asio::ip::tcp::socket * socket = new boost::asio::ip::tcp::socket(service);
    connect(*socket, iterator);

    boost::asio::streambuf request;
    ostream requestStream(&request);
    requestStream << "GET" << " " << "/";
    requestStream << " HTTP/1.0" << "\r\n";
    requestStream << "\r\n";

    write(*socket, request);

    return socket;
}

int main(int argc, char ** argv) {
    boost::asio::ip::tcp::socket * socket = getSocket();
    boost::asio::streambuf buffer;
    string line;
    while (readLine(*socket, buffer, line) && line != "") {
        cerr << line << endl;
    }
}
