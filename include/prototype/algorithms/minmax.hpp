///////////////////////////////////////////////////////////////////////////////
// include/prototype/algorithms/minmax.hpp
//
// PrototypeSTL - Min/max element algorithms
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALGORITHM_MINMAX_HPP
#define PROTOTYPE_ALGORITHM_MINMAX_HPP

#include "../config.hpp"
#include "../iterators/iterator_traits.hpp"
#include "../utility/utility.hpp"

namespace prototype {

///////////////////////////////////////////////////////////////////////////////
// min_element
///////////////////////////////////////////////////////////////////////////////
template<class ForwardIt, class Compare>
ForwardIt min_element(ForwardIt first, ForwardIt last, Compare comp) {
    if (first == last) return last;
    ForwardIt smallest = first;
    ++first;
    for (; first != last; ++first) {
        if (comp(*first, *smallest)) {
            smallest = first;
        }
    }
    return smallest;
}

template<class ForwardIt>
ForwardIt min_element(ForwardIt first, ForwardIt last) {
    typedef typename iterator_traits<ForwardIt>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    return min_element(first, last, Less());
}

///////////////////////////////////////////////////////////////////////////////
// max_element
///////////////////////////////////////////////////////////////////////////////
template<class ForwardIt, class Compare>
ForwardIt max_element(ForwardIt first, ForwardIt last, Compare comp) {
    if (first == last) return last;
    ForwardIt largest = first;
    ++first;
    for (; first != last; ++first) {
        if (comp(*largest, *first)) {
            largest = first;
        }
    }
    return largest;
}

template<class ForwardIt>
ForwardIt max_element(ForwardIt first, ForwardIt last) {
    typedef typename iterator_traits<ForwardIt>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    return max_element(first, last, Less());
}

///////////////////////////////////////////////////////////////////////////////
// minmax_element
///////////////////////////////////////////////////////////////////////////////
template<class ForwardIt, class Compare>
pair<ForwardIt, ForwardIt> minmax_element(ForwardIt first, ForwardIt last, Compare comp) {
    ForwardIt min_it = first;
    ForwardIt max_it = first;
    if (first == last) return make_pair(last, last);
    ++first;
    for (; first != last; ++first) {
        if (comp(*first, *min_it)) min_it = first;
        if (!comp(*first, *max_it)) max_it = first;
    }
    return make_pair(min_it, max_it);
}

template<class ForwardIt>
pair<ForwardIt, ForwardIt> minmax_element(ForwardIt first, ForwardIt last) {
    typedef typename iterator_traits<ForwardIt>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    return minmax_element(first, last, Less());
}

// Note: nth_element is defined in algorithms/partition.hpp

} // namespace prototype

#endif // PROTOTYPE_ALGORITHM_MINMAX_HPP
