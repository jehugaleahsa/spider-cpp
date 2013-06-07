#ifndef SPIDER_DOWNLOAD_MANAGER_HPP
#define SPIDER_DOWNLOAD_MANAGER_HPP

#include <functional>
#include <vector>
#include <utility>
#include <boost/optional.hpp>
#include "categorizer.hpp"
#include "download_factory.hpp"
#include "task_pool.hpp"
#include "tracker.hpp"
#include "url.hpp"

namespace spider {

    class DownloadManager {
        TaskPool & m_pool;
        UrlTracker & m_tracker;
        std::vector<std::pair<Categorizer const&, DownloadFactory const&>> m_categories;

    public:
        DownloadManager(TaskPool & pool, UrlTracker & tracker);

        void associate(Categorizer const& categorizer, DownloadFactory const& downloadFactory);

        template <typename TInputIterator>
        void download(
            boost::optional<Url> referrer, 
            TInputIterator first, TInputIterator past);
    };

    template <typename TInputIterator>
    void DownloadManager::download(
        boost::optional<Url> referrer, 
        TInputIterator first, 
        TInputIterator past) {
        using std::for_each;
        using std::function;
        using std::pair;

        for_each(first, past, [&](Url const& url) {
            if (m_tracker.addUrl(url)) {
                for_each(m_categories.begin(), m_categories.end(),
                    [&](pair<Categorizer const&, DownloadFactory const&> const& pair) {
                        Categorizer const& categorizer = pair.first;
                        if (categorizer.isDesired(url)) {
                            DownloadFactory const& factory = pair.second;
                            function<void(void)> task = factory.create(url, referrer);
                            int priority = categorizer.getPriority(url);
                            m_pool.addTask(priority, task);
                        }
                    });
            }
        });
    }

}

#endif  // SPIDER_DOWNLOAD_MANAGER_HPP
