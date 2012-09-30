#ifndef SPIDER_ALGORITHM_HPP
#define SPIDER_ALGORITHM_HPP

namespace spider {

    template <typename TInputIterator, typename TOutputIterator, typename TPredicate>
    TOutputIterator copy_while(
        TInputIterator first, TInputIterator past,
        TOutputIterator destination,
        TPredicate predicate) {
        for (; first != past && predicate(*first); ++first) {
           *destination = *first;
        }
        return destination;
    }

}

#endif // SPIDER_ALGORITHM_HPP
