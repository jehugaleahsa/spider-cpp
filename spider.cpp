#include <memory>
#include <vector>
#include <boost/optional.hpp>
#include "categorizer.hpp"
#include "counter.hpp"
#include "downloader.hpp"
#include "download_manager.hpp"
#include "extractor.hpp"
#include "file_download_factory.hpp"
#include "page_download_factory.hpp"
#include "spider.hpp"
#include "stripper.hpp"
#include "thread_pool.hpp"
#include "tracker.hpp"
#include "url.hpp"
#include "url_finder.hpp"

namespace {

    void supportPageExtensions(spider::Categorizer & categorizer) {
        categorizer.supportExtension(0, "");
        categorizer.supportExtension(0, "htm");
        categorizer.supportExtension(0, "html");
        categorizer.supportExtension(0, "xhtml");
        categorizer.supportExtension(0, "jsp");
        categorizer.supportExtension(0, "php");
        categorizer.supportExtension(0, "asp");
        categorizer.supportExtension(0, "aspx");
        categorizer.supportExtension(0, "cgi");
        categorizer.supportExtension(0, "shtml");
        categorizer.supportExtension(0, "cfm");
        categorizer.supportExtension(0, "cfml");
    }

    void supportMediaExtensions(spider::Categorizer & categorizer) {
        categorizer.supportExtension(2, "mpg");
        categorizer.supportExtension(2, "mpeg");
        categorizer.supportExtension(2, "mp4");
        categorizer.supportExtension(2, "avi");
        categorizer.supportExtension(2, "wmv");
        categorizer.supportExtension(2, "mov");
        categorizer.supportExtension(2, "rm");
        categorizer.supportExtension(1, "png");
        categorizer.supportExtension(1, "gif");
        categorizer.supportExtension(1, "jpg");
        categorizer.supportExtension(1, "tif");
        categorizer.supportExtension(1, "bmp"); 
    }

}

void spider::Spider::run(
    std::ostream & output, 
    Url const& topUrl,
    std::string const& downloadDirectory) const {
    using std::make_shared;
    using std::vector;
    using boost::optional;

    int processorCount = getProcessorCount();
    Counter counter;
    ThreadPool pool(counter, processorCount + 2);
    pool.start();
    
    UrlTracker tracker;

    DownloadManager manager(pool, tracker);
    
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
    
    PageDownloadFactory pageFactory(manager, finder);
    Categorizer pageCategorizer;
    supportPageExtensions(pageCategorizer);
    manager.associate(pageCategorizer, pageFactory);
    
    FileDownloadFactory fileFactory(downloadDirectory);
    Categorizer mediaCategorizer;
    supportMediaExtensions(mediaCategorizer);
    manager.associate(mediaCategorizer, fileFactory);

    vector<Url> rootUrls { topUrl };
    manager.download(optional<Url>(), rootUrls.begin(), rootUrls.end());

    counter.wait();
}
