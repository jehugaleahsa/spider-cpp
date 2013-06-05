#ifndef SPIDER_DOWNLOAD_MANAGER_HPP
#define SPIDER_DOWNLOAD_MANAGER_HPP

#include <functional>
#include <vector>
#include <utility>
#include "categorizer.hpp"
#include "download_factory.hpp"
#include "thread_pool.hpp"
#include "tracker.hpp"
#include "url.hpp"

namespace spider {

    class DownloadManager {
        ThreadPool & m_pool;
        UrlTracker & m_tracker;
        std::vector<std::pair<Categorizer const&, DownloadFactory const&>> m_categories;

    public:
        DownloadManager(ThreadPool & pool, UrlTracker & tracker);

        void associate(Categorizer const& categorizer, DownloadFactory const& downloadFactory);

        template <typename TInputIterator>
        void download(Url const& referrer, TInputIterator first, TInputIterator past);
    };

    template <typename TInputIterator>
    void DownloadManager::download(
        Url const& referrer, 
        TInputIterator first, 
        TInputIterator past) {
        for_each(first, past,
            [&](Url const& url) {
                if (m_tracker.addUrl(url)) {
                    for_each(m_categories.begin(), m_categories.end(),
                        [&](std::pair<Categorizer const&, DownloadFactory const&> const& pair) {
                            Categorizer const& categorizer = pair.first;
                            if (categorizer.isDesired(url)) {
                                DownloadFactory const& factory = pair.second;
                                std::function<void(void)> task = factory.create(url, referrer);
                                m_pool.addTask(task);
                            }
                        });
                }
            });
    }

}

#endif  // SPIDER_DOWNLOAD_MANAGER_HPP
