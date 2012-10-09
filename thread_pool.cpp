#include <boost/thread.hpp>
#include "thread_pool.hpp"

namespace spider {

    int getProcessorCount() {
        return boost::thread::hardware_concurrency();
    }

}
