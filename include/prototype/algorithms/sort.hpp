///////////////////////////////////////////////////////////////////////////////
// include/prototype/algorithms/sort.hpp
//
// PrototypeSTL - insertion_sort, introsort (sort)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALGORITHM_SORT_HPP
#define PROTOTYPE_ALGORITHM_SORT_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"
#include "../iterators/iterator_traits.hpp"

namespace prototype {

namespace detail {

template<class RandomIt, class Compare>
inline void insertion_sort(RandomIt first, RandomIt last, Compare comp) {
    if (first == last) return;
    for (RandomIt i = first + 1; i != last; ++i) {
        typename iterator_traits<RandomIt>::value_type key = prototype::move(*i);
        RandomIt j = i;
        while (j != first && comp(key, *(j - 1))) {
            *j = prototype::move(*(j - 1));
            --j;
        }
        *j = prototype::move(key);
    }
}

template<class RandomIt, class Compare>
inline RandomIt median_of_three(RandomIt first, RandomIt last, Compare comp) {
    RandomIt mid = first + (last - first) / 2;
    RandomIt back = last - 1;
    if (comp(*mid, *first)) prototype::swap(*first, *mid);
    if (comp(*back, *first)) prototype::swap(*first, *back);
    if (comp(*back, *mid)) prototype::swap(*mid, *back);
    return mid;
}

template<class RandomIt, class Compare>
inline void introsort_impl(RandomIt first, RandomIt last, int depth_limit, Compare comp) {
    while (last - first > 16) {
        if (depth_limit == 0) {
            // Fall back to insertion sort for small partitions
            detail::insertion_sort(first, last, comp);
            return;
        }
        --depth_limit;

        // Partition
        RandomIt pivot = median_of_three(first, last, comp);
        typename iterator_traits<RandomIt>::value_type pval = *pivot;
        prototype::swap(*pivot, *(last - 1));

        RandomIt store = first;
        for (RandomIt it = first; it != last - 1; ++it) {
            if (comp(*it, pval)) {
                prototype::swap(*it, *store);
                ++store;
            }
        }
        prototype::swap(*store, *(last - 1));

        // Recurse on smaller partition, iterate on larger
        if (store - first < last - store - 1) {
            introsort_impl(first, store, depth_limit, comp);
            first = store + 1;
        } else {
            introsort_impl(store + 1, last, depth_limit, comp);
            last = store;
        }
    }
    detail::insertion_sort(first, last, comp);
}

inline int log2_floor(size_t n) {
    int r = 0;
    while (n > 1) { n >>= 1; ++r; }
    return r;
}

} // namespace detail

template<class RandomIt, class Compare>
inline void sort(RandomIt first, RandomIt last, Compare comp) {
    if (first == last) return;
    size_t n = static_cast<size_t>(last - first);
    int depth = detail::log2_floor(n) * 2;
    detail::introsort_impl(first, last, depth, comp);
}

template<class RandomIt>
inline void sort(RandomIt first, RandomIt last) {
    typedef typename iterator_traits<RandomIt>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    sort(first, last, Less());
}

template<class RandomIt, class Compare>
inline void insertion_sort(RandomIt first, RandomIt last, Compare comp) {
    detail::insertion_sort(first, last, comp);
}

template<class RandomIt>
inline void insertion_sort(RandomIt first, RandomIt last) {
    typedef typename iterator_traits<RandomIt>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    detail::insertion_sort(first, last, Less());
}

} // namespace prototype

#endif // PROTOTYPE_ALGORITHM_SORT_HPP
