#ifndef SPIDER_ENVIRONMENT_HPP
#define SPIDER_ENVIRONMENT_HPP

namespace spider {

    class Environment {
        Environment();
        Environment(Environment const& other);
        Environment & operator=(Environment const& other);

    public:
        static int getProcessorCount();
    };

}
#endif // SPIDER_ENVIRONMENT_HPP
