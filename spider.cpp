#include <memory>
#include <vector>
#include <boost/optional.hpp>
#include "categorizer.hpp"
#include "downloader.hpp"
#include "download_manager.hpp"
#include "extractor.hpp"
#include "file_download_factory.hpp"
#include "page_download_factory.hpp"
#include "spider.hpp"
#include "stripper.hpp"
#include "task_pool.hpp"
#include "tracker.hpp"
#include "url.hpp"
#include "url_finder.hpp"

void spider::Spider::run(
    Url const& topUrl,
    std::string const& downloadDirectory,
    Categorizer const& pageCategorizer,
    Categorizer const& mediaCategorizer) const {
    using std::make_shared;
    using std::vector;
    using boost::optional;

    int processorCount = getProcessorCount();
    //TestPool pool;
    ThreadPool pool(processorCount + 2);
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
    manager.associate(pageCategorizer, pageFactory);
    
    FileDownloadFactory fileFactory(downloadDirectory);
    manager.associate(mediaCategorizer, fileFactory);

    vector<Url> rootUrls { topUrl };
    manager.download(optional<Url>(), rootUrls.begin(), rootUrls.end());

    pool.wait();
}
