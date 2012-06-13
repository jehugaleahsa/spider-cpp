#ifndef SPIDER_ALGORITHM_HPP
#define SPIDER_ALGORITHM_HPP

namespace spider {

template <typename TInIterator, typename TOutIterator, typename TPredicate, typename TTransformer>
TOutIterator transform_while(TInIterator first, TInIterator past, TOutIterator destination, TPredicate predicate, TTransformer transformer) {
    while (first != past && predicate(*first)) {
        *destination++ = transformer(*first++);
    }
    return destination;
}

}

#endif // SPIDER_ALGORITHM_HPP
