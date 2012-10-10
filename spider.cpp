#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
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

namespace spider {

    void Spider::run(std::ostream & output, Url const& topUrl) const {
        using boost::function;
        using boost::shared_ptr;

        Categorizer pageCategorizer;
        supportPageExtensions(pageCategorizer);

        Categorizer mediaCategorizer;
        supportMediaExtensions(mediaCategorizer);

        Stripper stripper("script");
        UrlExtractor baseExtractor("base", "href");
        UrlExtractor anchorExtractor("a", "href");
        UrlExtractor imageExtractor("img", "src");

        Counter counter;
        int processorCount = getProcessorCount();
        ThreadPool pool(processorCount + 2);
        pool.start();
        
        UrlTracker tracker;
        tracker.addUrl(topUrl);

        {
            shared_ptr<Downloadable> home(new PageDownloadable(
                counter,
                topUrl,
                topUrl,
                pool,
                tracker,
                pageCategorizer,
                mediaCategorizer,
                stripper,
                baseExtractor,
                anchorExtractor,
                imageExtractor
            ));
            pool.addTask(bind(&Downloadable::download, home));
        }

        counter.wait();
    }

}
