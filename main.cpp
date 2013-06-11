#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include "categorizer.hpp"
#include "spider.hpp"
#include "url.hpp"

std::string getStartingUrl() {
    using std::cin;
    using std::cout;
    using std::string;

    cout << "Please enter the URL: ";
    string urlString;
    cin >> urlString;
    return urlString;
}

std::string getDownloadDirectory() {
    using std::cin;
    using std::cout;
    using std::string;

    cout << "Please enter the download directory: ";
    string downloadDirectory;
    cin >> downloadDirectory;
    return downloadDirectory;
}

void buildCategorizer(std::string const& extensions, spider::Categorizer & categorizer, int defaultPriority) {
    using std::back_inserter;
    using std::find;
    using std::istringstream;
    using std::pair;
    using std::string;
    using std::transform;
    using std::vector;
    using boost::is_any_of;
    using boost::split;
    using boost::starts_with;
    using boost::trim_left_copy_if;

    vector<string> descriptions;
    split(descriptions, extensions, is_any_of(","));

    for_each(
        descriptions.begin(), descriptions.end(), 
        [&, defaultPriority](string const& description) {
            string::const_iterator position = find(description.begin(), description.end(), ':');
            string extension(description.begin(), position);
            if (starts_with(extension, ".")) {
                extension = trim_left_copy_if(extension, is_any_of("."));
            }
            int priority = defaultPriority;
            if (position != description.end()) {
                istringstream converter(string(position, description.end()));
                converter >> priority;
            }
            categorizer.supportExtension(priority, extension);
        });
}

int main(int argc, char** argv) {
    using std::cout;
    using std::endl;
    using std::string;
    using boost::program_options::notify;
    using boost::program_options::options_description;
    using boost::program_options::parse_command_line;
    using boost::program_options::store;
    using boost::program_options::value;
    using boost::program_options::variables_map;
    using spider::Categorizer;
    using spider::Spider;
    using spider::Url;

    int threadCount = 0;
    string pageExtensions;
    string mediaExtensions;
    string downloadDirectory;
    string urlString;

    options_description description("Usage: spider [options]");
    description.add_options()
        ("directory,d", value<string>(&downloadDirectory)->default_value(string()), "The directory to save downloaded media.")
        ("help,h", "Display this help message.")
        ("media-extensions,m", value<string>(&mediaExtensions)->default_value(string()), "Comma-separated list of valid media extensions.")
        ("page-extensions,p", value<string>(&pageExtensions)->default_value(string()), "Comma-separated list of valid page extensions.")
        ("threads,t", value<int>(&threadCount)->default_value(0), "The number of threads to create.")
        ("url,u", value<string>(&urlString), "The initial URL to begin searching for media.");

    variables_map map;
    store(parse_command_line(argc, argv, description), map);
    notify(map);

    if (map.count("help")) {
        cout << description << endl;
        return 1;
    }

    if (urlString.empty()) {
        urlString = getStartingUrl();
    }
    if (downloadDirectory.empty()) {
        downloadDirectory = getDownloadDirectory();
    }
    if (threadCount < 0) {
        threadCount = 0;
    }
    if (mediaExtensions.empty()) {
        mediaExtensions = "mpg:2,mpeg:2,mp4:2,avi:2,wmv:2,mov:2,rm:2,png:1,gif:1,jpg:1,tif:1,bmp:1,pdf:1";
    }
    if (pageExtensions.empty()) {
        pageExtensions = ":0,htm:0,html:0,xhtml:0,shtml:0,jsp:0,php:0,asp:0,aspx:0,cgi:0,cfm:0,cfml:0";
    }

    Categorizer pageCategorizer;
    buildCategorizer(pageExtensions, pageCategorizer, 0);
    Categorizer mediaCategorizer;
    buildCategorizer(mediaExtensions, mediaCategorizer, 1);

    Url url = Url::parse(urlString);
    Spider spider;
    spider.run(url, downloadDirectory, pageCategorizer, mediaCategorizer);

    return 0;
}
