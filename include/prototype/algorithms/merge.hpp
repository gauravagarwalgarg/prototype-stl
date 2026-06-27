///////////////////////////////////////////////////////////////////////////////
// include/prototype/algorithms/merge.hpp
//
// PrototypeSTL - merge, inplace_merge
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALGORITHM_MERGE_HPP
#define PROTOTYPE_ALGORITHM_MERGE_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"
#include "rotate.hpp"

namespace prototype {

template<class InputIt1, class InputIt2, class OutputIt>
inline OutputIt merge(InputIt1 first1, InputIt1 last1,
                      InputIt2 first2, InputIt2 last2,
                      OutputIt d_first) {
    for (; first1 != last1; ++d_first) {
        if (first2 == last2) {
            for (; first1 != last1; ++first1, ++d_first)
                *d_first = *first1;
            return d_first;
        }
        if (*first2 < *first1) {
            *d_first = *first2;
            ++first2;
        } else {
            *d_first = *first1;
            ++first1;
        }
    }
    for (; first2 != last2; ++first2, ++d_first)
        *d_first = *first2;
    return d_first;
}

template<class InputIt1, class InputIt2, class OutputIt, class Compare>
inline OutputIt merge(InputIt1 first1, InputIt1 last1,
                      InputIt2 first2, InputIt2 last2,
                      OutputIt d_first, Compare comp) {
    for (; first1 != last1; ++d_first) {
        if (first2 == last2) {
            for (; first1 != last1; ++first1, ++d_first)
                *d_first = *first1;
            return d_first;
        }
        if (comp(*first2, *first1)) {
            *d_first = *first2;
            ++first2;
        } else {
            *d_first = *first1;
            ++first1;
        }
    }
    for (; first2 != last2; ++first2, ++d_first)
        *d_first = *first2;
    return d_first;
}

// In-place merge using rotation (no extra memory needed)
template<class BidirIt>
inline void inplace_merge(BidirIt first, BidirIt middle, BidirIt last) {
    if (first == middle || middle == last) return;

    BidirIt left = first;
    BidirIt right = middle;

    while (left != right && right != last) {
        while (left != right && !(*right < *left)) ++left;
        BidirIt start = right;
        while (right != last && *right < *left) ++right;
        prototype::rotate(left, start, right);
        // Adjust left past the rotated-in elements
        auto dist = right;
        for (BidirIt t = start; t != right; ++t) ++left;
        // left already advanced correctly via rotate return, but we use simple approach
    }
}

} // namespace prototype

#endif // PROTOTYPE_ALGORITHM_MERGE_HPP
