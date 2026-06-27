///////////////////////////////////////////////////////////////////////////////
// include/prototype/algorithms/binary_search.hpp
//
// PrototypeSTL - Binary search algorithms on sorted ranges
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALGORITHM_BINARY_SEARCH_HPP
#define PROTOTYPE_ALGORITHM_BINARY_SEARCH_HPP

#include "../config.hpp"
#include "../iterators/iterator_traits.hpp"

namespace prototype {

///////////////////////////////////////////////////////////////////////////////
// lower_bound - first element not less than value
///////////////////////////////////////////////////////////////////////////////
template<class ForwardIt, class T, class Compare>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
    typedef typename iterator_traits<ForwardIt>::difference_type diff_t;
    diff_t count = last - first;
    while (count > 0) {
        diff_t step = count / 2;
        ForwardIt mid = first + step;
        if (comp(*mid, value)) {
            first = mid + 1;
            count -= step + 1;
        } else {
            count = step;
        }
    }
    return first;
}

template<class ForwardIt, class T>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value) {
    struct Less {
        bool operator()(const T& a, const T& b) const { return a < b; }
    };
    return lower_bound(first, last, value, Less());
}

///////////////////////////////////////////////////////////////////////////////
// upper_bound - first element greater than value
///////////////////////////////////////////////////////////////////////////////
template<class ForwardIt, class T, class Compare>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
    typedef typename iterator_traits<ForwardIt>::difference_type diff_t;
    diff_t count = last - first;
    while (count > 0) {
        diff_t step = count / 2;
        ForwardIt mid = first + step;
        if (!comp(value, *mid)) {
            first = mid + 1;
            count -= step + 1;
        } else {
            count = step;
        }
    }
    return first;
}

template<class ForwardIt, class T>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value) {
    struct Less {
        bool operator()(const T& a, const T& b) const { return a < b; }
    };
    return upper_bound(first, last, value, Less());
}

///////////////////////////////////////////////////////////////////////////////
// binary_search - check if value exists in sorted range
///////////////////////////////////////////////////////////////////////////////
template<class ForwardIt, class T, class Compare>
bool binary_search(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
    ForwardIt it = lower_bound(first, last, value, comp);
    return (it != last && !comp(value, *it));
}

template<class ForwardIt, class T>
bool binary_search(ForwardIt first, ForwardIt last, const T& value) {
    struct Less {
        bool operator()(const T& a, const T& b) const { return a < b; }
    };
    return binary_search(first, last, value, Less());
}

///////////////////////////////////////////////////////////////////////////////
// equal_range - pair of [lower_bound, upper_bound)
///////////////////////////////////////////////////////////////////////////////
template<class ForwardIt, class T, class Compare>
pair<ForwardIt, ForwardIt> equal_range(ForwardIt first, ForwardIt last,
                                        const T& value, Compare comp) {
    return make_pair(lower_bound(first, last, value, comp),
                     upper_bound(first, last, value, comp));
}

template<class ForwardIt, class T>
pair<ForwardIt, ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T& value) {
    struct Less {
        bool operator()(const T& a, const T& b) const { return a < b; }
    };
    return equal_range(first, last, value, Less());
}

} // namespace prototype

#endif // PROTOTYPE_ALGORITHM_BINARY_SEARCH_HPP
