///////////////////////////////////////////////////////////////////////////////
// include/prototype/algorithms/heap.hpp
//
// PrototypeSTL - push_heap, pop_heap, make_heap, sort_heap
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALGORITHM_HEAP_HPP
#define PROTOTYPE_ALGORITHM_HEAP_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"
#include "../iterators/iterator_traits.hpp"

namespace prototype {

namespace detail {

template<class RandomIt, class Compare>
inline void sift_up(RandomIt first, typename iterator_traits<RandomIt>::difference_type idx, Compare comp) {
    typedef typename iterator_traits<RandomIt>::value_type value_type;
    value_type val = prototype::move(*(first + idx));
    while (idx > 0) {
        auto parent = (idx - 1) / 2;
        if (comp(*(first + parent), val)) {
            *(first + idx) = prototype::move(*(first + parent));
            idx = parent;
        } else {
            break;
        }
    }
    *(first + idx) = prototype::move(val);
}

template<class RandomIt, class Compare>
inline void sift_down(RandomIt first, typename iterator_traits<RandomIt>::difference_type idx,
                      typename iterator_traits<RandomIt>::difference_type len, Compare comp) {
    typedef typename iterator_traits<RandomIt>::value_type value_type;
    value_type val = prototype::move(*(first + idx));
    auto child = idx * 2 + 1;
    while (child < len) {
        if (child + 1 < len && comp(*(first + child), *(first + child + 1))) {
            ++child;
        }
        if (comp(val, *(first + child))) {
            *(first + idx) = prototype::move(*(first + child));
            idx = child;
            child = idx * 2 + 1;
        } else {
            break;
        }
    }
    *(first + idx) = prototype::move(val);
}

} // namespace detail

template<class RandomIt, class Compare>
inline void push_heap(RandomIt first, RandomIt last, Compare comp) {
    auto n = last - first;
    if (n > 1) {
        detail::sift_up(first, n - 1, comp);
    }
}

template<class RandomIt>
inline void push_heap(RandomIt first, RandomIt last) {
    typedef typename iterator_traits<RandomIt>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    push_heap(first, last, Less());
}

template<class RandomIt, class Compare>
inline void pop_heap(RandomIt first, RandomIt last, Compare comp) {
    auto n = last - first;
    if (n > 1) {
        prototype::swap(*first, *(last - 1));
        detail::sift_down(first, (decltype(n))0, n - 1, comp);
    }
}

template<class RandomIt>
inline void pop_heap(RandomIt first, RandomIt last) {
    typedef typename iterator_traits<RandomIt>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    pop_heap(first, last, Less());
}

template<class RandomIt, class Compare>
inline void make_heap(RandomIt first, RandomIt last, Compare comp) {
    auto n = last - first;
    for (auto i = n / 2 - 1; i >= 0; --i) {
        detail::sift_down(first, i, n, comp);
    }
}

template<class RandomIt>
inline void make_heap(RandomIt first, RandomIt last) {
    typedef typename iterator_traits<RandomIt>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    make_heap(first, last, Less());
}

template<class RandomIt, class Compare>
inline void sort_heap(RandomIt first, RandomIt last, Compare comp) {
    while (last - first > 1) {
        pop_heap(first, last, comp);
        --last;
    }
}

template<class RandomIt>
inline void sort_heap(RandomIt first, RandomIt last) {
    typedef typename iterator_traits<RandomIt>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    sort_heap(first, last, Less());
}

} // namespace prototype

#endif // PROTOTYPE_ALGORITHM_HEAP_HPP
