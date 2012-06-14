#ifndef SPIDER_ALGORITHM_HPP
#define SPIDER_ALGORITHM_HPP

namespace spider {

    template <typename TInputIterator, typename TPredicate, typename TAction>
    void for_each_while(TInputIterator first, TInputIterator past, TPredicate predicate, TAction action) {
         for (; first != past && predicate(*first); ++first) {
            action(*first);
         }   
    }

}

#endif // SPIDER_ALGORITHM_HPP
