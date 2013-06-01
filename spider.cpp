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

    void Spider::run(
        std::ostream & output, 
        Url const& topUrl,
        std::string const& downloadDirectory) const {
        using boost::function;
        using boost::shared_ptr;

        Categorizer pageCategorizer;
        supportPageExtensions(pageCategorizer);

        Categorizer mediaCategorizer;
        supportMediaExtensions(mediaCategorizer);

        Stripper stripper("script");
        TagUrlExtractor baseExtractor("base", "href");
        shared_ptr<UrlExtractor> anchorExtractor(new TagUrlExtractor("a", "href"));
        shared_ptr<UrlExtractor> imageExtractor(new TagUrlExtractor("img", "src"));
        shared_ptr<UrlExtractor> videoExtractor(new TagUrlExtractor("video", "src"));
        shared_ptr<UrlExtractor> sourceExtractor(new TagUrlExtractor("source", "src"));
        shared_ptr<UrlExtractor> embedExtractor(new TagUrlExtractor("embed", "flashvars"));
        shared_ptr<UrlExtractor> paramExtractor(new TagUrlExtractor("param", "value"));
        CompoundExtractor extractor;
        extractor.addExtractor(anchorExtractor);
        extractor.addExtractor(imageExtractor);
        extractor.addExtractor(videoExtractor);
        extractor.addExtractor(sourceExtractor);
        extractor.addExtractor(embedExtractor);
        extractor.addExtractor(paramExtractor);


        Counter counter;
        int processorCount = getProcessorCount();
        ThreadPool pool(processorCount + 2);
        pool.start();
        
        UrlTracker tracker;
        tracker.addUrl(topUrl);

        {
            shared_ptr<Downloader> home(new PageDownloader(
                counter,
                topUrl,
                shared_ptr<Url>(),
                downloadDirectory,
                pool,
                tracker,
                pageCategorizer,
                mediaCategorizer,
                stripper,
                baseExtractor,
                extractor
            ));
            pool.addTask(bind(&Downloader::download, home));
        }

        counter.wait();
    }

}
