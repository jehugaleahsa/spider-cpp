#include <memory>
#include "categorizer.hpp"
#include "counter.hpp"
#include "downloader.hpp"
#include "extractor.hpp"
#include "page_downloader.hpp"
#include "spider.hpp"
#include "stripper.hpp"
#include "thread_pool.hpp"
#include "tracker.hpp"
#include "url.hpp"
#include "url_finder.hpp"

namespace {

    void supportPageExtensions(spider::Categorizer & categorizer) {
        categorizer.supportExtension("");
        categorizer.supportExtension("htm");
        categorizer.supportExtension("html");
        categorizer.supportExtension("xhtml");
        categorizer.supportExtension("jsp");
        categorizer.supportExtension("php");
        categorizer.supportExtension("asp");
        categorizer.supportExtension("aspx");
        categorizer.supportExtension("cgi");
        categorizer.supportExtension("shtml");
        categorizer.supportExtension("cfm");
        categorizer.supportExtension("cfml");
    }

    void supportMediaExtensions(spider::Categorizer & categorizer) {
        categorizer.supportExtension("mpg");
        categorizer.supportExtension("mpeg");
        categorizer.supportExtension("mp4");
        categorizer.supportExtension("avi");
        categorizer.supportExtension("wmv");
        categorizer.supportExtension("mov");
        categorizer.supportExtension("rm");
        categorizer.supportExtension("png");
        categorizer.supportExtension("gif");
        categorizer.supportExtension("jpg");
        categorizer.supportExtension("tif");
        categorizer.supportExtension("bmp"); 
    }

}

void spider::Spider::run(
    std::ostream & output, 
    Url const& topUrl,
    std::string const& downloadDirectory) const {
    using std::make_shared;

    Categorizer pageCategorizer;
    supportPageExtensions(pageCategorizer);

    Categorizer mediaCategorizer;
    supportMediaExtensions(mediaCategorizer);

    Stripper stripper("script");
    TagUrlExtractor baseExtractor("base", "href");
    CompoundExtractor extractor;
    extractor.addExtractor(make_shared<TagUrlExtractor>("a", "href"));
    extractor.addExtractor(make_shared<TagUrlExtractor>("img", "src"));
    extractor.addExtractor(make_shared<TagUrlExtractor>("video", "src"));
    extractor.addExtractor(make_shared<TagUrlExtractor>("source", "src"));
    extractor.addExtractor(make_shared<TagUrlExtractor>("embed", "flashvars"));
    extractor.addExtractor(make_shared<TagUrlExtractor>("param", "value"));
    UrlFinder finder(stripper, baseExtractor, extractor);

    int processorCount = getProcessorCount();
    Counter counter;
    ThreadPool pool(counter, processorCount + 2);
    pool.start();
    
    UrlTracker tracker;
    tracker.addUrl(topUrl);

    pool.addTask([&]() {
        PageDownloader home(topUrl, Url());
        home.download(
            downloadDirectory,
            pool,
            tracker,
            pageCategorizer,
            mediaCategorizer,
            finder);
    });

    counter.wait();
}
