///////////////////////////////////////////////////////////////////////////////
// include/prototype/algorithms/set_operations.hpp
//
// PrototypeSTL - Set operations on sorted ranges
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALGORITHM_SET_OPERATIONS_HPP
#define PROTOTYPE_ALGORITHM_SET_OPERATIONS_HPP

#include "../config.hpp"
#include "../iterators/iterator_traits.hpp"

namespace prototype {

///////////////////////////////////////////////////////////////////////////////
// includes - test if sorted range [first1,last1) contains all of [first2,last2)
///////////////////////////////////////////////////////////////////////////////
template<class InputIt1, class InputIt2, class Compare>
bool includes(InputIt1 first1, InputIt1 last1,
              InputIt2 first2, InputIt2 last2, Compare comp) {
    while (first2 != last2) {
        if (first1 == last1) return false;
        if (comp(*first2, *first1)) return false;
        if (!comp(*first1, *first2)) {
            ++first2;
        }
        ++first1;
    }
    return true;
}

template<class InputIt1, class InputIt2>
bool includes(InputIt1 first1, InputIt1 last1,
              InputIt2 first2, InputIt2 last2) {
    typedef typename iterator_traits<InputIt1>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    return includes(first1, last1, first2, last2, Less());
}

///////////////////////////////////////////////////////////////////////////////
// set_union
///////////////////////////////////////////////////////////////////////////////
template<class InputIt1, class InputIt2, class OutputIt, class Compare>
OutputIt set_union(InputIt1 first1, InputIt1 last1,
                   InputIt2 first2, InputIt2 last2,
                   OutputIt d_first, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *d_first = *first1;
            ++first1;
        } else if (comp(*first2, *first1)) {
            *d_first = *first2;
            ++first2;
        } else {
            *d_first = *first1;
            ++first1;
            ++first2;
        }
        ++d_first;
    }
    while (first1 != last1) { *d_first = *first1; ++d_first; ++first1; }
    while (first2 != last2) { *d_first = *first2; ++d_first; ++first2; }
    return d_first;
}

template<class InputIt1, class InputIt2, class OutputIt>
OutputIt set_union(InputIt1 first1, InputIt1 last1,
                   InputIt2 first2, InputIt2 last2,
                   OutputIt d_first) {
    typedef typename iterator_traits<InputIt1>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    return set_union(first1, last1, first2, last2, d_first, Less());
}

///////////////////////////////////////////////////////////////////////////////
// set_intersection
///////////////////////////////////////////////////////////////////////////////
template<class InputIt1, class InputIt2, class OutputIt, class Compare>
OutputIt set_intersection(InputIt1 first1, InputIt1 last1,
                          InputIt2 first2, InputIt2 last2,
                          OutputIt d_first, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            ++first1;
        } else if (comp(*first2, *first1)) {
            ++first2;
        } else {
            *d_first = *first1;
            ++d_first;
            ++first1;
            ++first2;
        }
    }
    return d_first;
}

template<class InputIt1, class InputIt2, class OutputIt>
OutputIt set_intersection(InputIt1 first1, InputIt1 last1,
                          InputIt2 first2, InputIt2 last2,
                          OutputIt d_first) {
    typedef typename iterator_traits<InputIt1>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    return set_intersection(first1, last1, first2, last2, d_first, Less());
}

///////////////////////////////////////////////////////////////////////////////
// set_difference
///////////////////////////////////////////////////////////////////////////////
template<class InputIt1, class InputIt2, class OutputIt, class Compare>
OutputIt set_difference(InputIt1 first1, InputIt1 last1,
                        InputIt2 first2, InputIt2 last2,
                        OutputIt d_first, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *d_first = *first1;
            ++d_first;
            ++first1;
        } else if (comp(*first2, *first1)) {
            ++first2;
        } else {
            ++first1;
            ++first2;
        }
    }
    while (first1 != last1) { *d_first = *first1; ++d_first; ++first1; }
    return d_first;
}

template<class InputIt1, class InputIt2, class OutputIt>
OutputIt set_difference(InputIt1 first1, InputIt1 last1,
                        InputIt2 first2, InputIt2 last2,
                        OutputIt d_first) {
    typedef typename iterator_traits<InputIt1>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    return set_difference(first1, last1, first2, last2, d_first, Less());
}

///////////////////////////////////////////////////////////////////////////////
// set_symmetric_difference
///////////////////////////////////////////////////////////////////////////////
template<class InputIt1, class InputIt2, class OutputIt, class Compare>
OutputIt set_symmetric_difference(InputIt1 first1, InputIt1 last1,
                                  InputIt2 first2, InputIt2 last2,
                                  OutputIt d_first, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *d_first = *first1;
            ++d_first;
            ++first1;
        } else if (comp(*first2, *first1)) {
            *d_first = *first2;
            ++d_first;
            ++first2;
        } else {
            ++first1;
            ++first2;
        }
    }
    while (first1 != last1) { *d_first = *first1; ++d_first; ++first1; }
    while (first2 != last2) { *d_first = *first2; ++d_first; ++first2; }
    return d_first;
}

template<class InputIt1, class InputIt2, class OutputIt>
OutputIt set_symmetric_difference(InputIt1 first1, InputIt1 last1,
                                  InputIt2 first2, InputIt2 last2,
                                  OutputIt d_first) {
    typedef typename iterator_traits<InputIt1>::value_type T;
    struct Less { bool operator()(const T& a, const T& b) const { return a < b; } };
    return set_symmetric_difference(first1, last1, first2, last2, d_first, Less());
}

} // namespace prototype

#endif // PROTOTYPE_ALGORITHM_SET_OPERATIONS_HPP
