///////////////////////////////////////////////////////////////////////////////
// include/prototype/algorithms/partition.hpp
//
// PrototypeSTL - partition, nth_element
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALGORITHM_PARTITION_HPP
#define PROTOTYPE_ALGORITHM_PARTITION_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"
#include "../iterators/iterator_traits.hpp"

namespace prototype {

template<class ForwardIt, class UnaryPredicate>
inline ForwardIt partition(ForwardIt first, ForwardIt last, UnaryPredicate pred) {
    // Find first element that doesn't satisfy predicate
    while (first != last && pred(*first)) ++first;
    if (first == last) return first;

    for (ForwardIt it = first; ++it != last;) {
        if (pred(*it)) {
            prototype::swap(*it, *first);
            ++first;
        }
    }
    return first;
}

template<class ForwardIt, class UnaryPredicate>
inline bool is_partitioned(ForwardIt first, ForwardIt last, UnaryPredicate pred) {
    while (first != last && pred(*first)) ++first;
    while (first != last) {
        if (pred(*first)) return false;
        ++first;
    }
    return true;
}

namespace detail {

template<class RandomIt, class Compare>
inline void nth_element_impl(RandomIt first, RandomIt nth, RandomIt last, Compare comp) {
    while (last - first > 1) {
        // Simple pivot selection: use middle element
        RandomIt mid = first + (last - first) / 2;
        if (comp(*mid, *first)) prototype::swap(*first, *mid);
        if (comp(*(last-1), *first)) prototype::swap(*first, *(last-1));
        if (comp(*(last-1), *mid)) prototype::swap(*mid, *(last-1));

        typename iterator_traits<RandomIt>::value_type pivot = *mid;
        prototype::swap(*mid, *(last - 1));

        RandomIt store = first;
        for (RandomIt it = first; it != last - 1; ++it) {
            if (comp(*it, pivot)) {
                prototype::swap(*it, *store);
                ++store;
            }
        }
        prototype::swap(*store, *(last - 1));

        if (store == nth) return;
        else if (nth < store) last = store;
        else first = store + 1;
    }
}

} // namespace detail

template<class RandomIt, class Compare>
inline void nth_element(RandomIt first, RandomIt nth, RandomIt last, Compare comp) {
    if (first == last || nth == last) return;
    detail::nth_element_impl(first, nth, last, comp);
}

template<class RandomIt>
inline void nth_element(RandomIt first, RandomIt nth, RandomIt last) {
    typedef typename iterator_traits<RandomIt>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    nth_element(first, nth, last, Less());
}

} // namespace prototype

#endif // PROTOTYPE_ALGORITHM_PARTITION_HPP
