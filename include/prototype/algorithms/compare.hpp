///////////////////////////////////////////////////////////////////////////////
// include/prototype/algorithms/compare.hpp
//
// PrototypeSTL - equal, lexicographical_compare, mismatch
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALGORITHM_COMPARE_HPP
#define PROTOTYPE_ALGORITHM_COMPARE_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"

namespace prototype {

template<class InputIt1, class InputIt2>
inline bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
    for (; first1 != last1; ++first1, ++first2) {
        if (!(*first1 == *first2)) return false;
    }
    return true;
}

template<class InputIt1, class InputIt2, class BinaryPredicate>
inline bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate pred) {
    for (; first1 != last1; ++first1, ++first2) {
        if (!pred(*first1, *first2)) return false;
    }
    return true;
}

template<class InputIt1, class InputIt2>
inline bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                                    InputIt2 first2, InputIt2 last2) {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2) return true;
        if (*first2 < *first1) return false;
    }
    return (first1 == last1) && (first2 != last2);
}

template<class InputIt1, class InputIt2>
inline pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
    while (first1 != last1 && *first1 == *first2) {
        ++first1;
        ++first2;
    }
    return make_pair(first1, first2);
}

} // namespace prototype

#endif // PROTOTYPE_ALGORITHM_COMPARE_HPP
