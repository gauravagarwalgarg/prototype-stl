///////////////////////////////////////////////////////////////////////////////
// include/prototype/algorithms/find.hpp
//
// PrototypeSTL - find, find_if, count, count_if
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALGORITHM_FIND_HPP
#define PROTOTYPE_ALGORITHM_FIND_HPP

#include "../config.hpp"
#include "../iterators/iterator_traits.hpp"

namespace prototype {

template<class InputIt, class T>
inline InputIt find(InputIt first, InputIt last, const T& value) {
    for (; first != last; ++first) {
        if (*first == value) return first;
    }
    return last;
}

template<class InputIt, class UnaryPredicate>
inline InputIt find_if(InputIt first, InputIt last, UnaryPredicate pred) {
    for (; first != last; ++first) {
        if (pred(*first)) return first;
    }
    return last;
}

template<class InputIt, class UnaryPredicate>
inline InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate pred) {
    for (; first != last; ++first) {
        if (!pred(*first)) return first;
    }
    return last;
}

template<class InputIt, class T>
inline typename iterator_traits<InputIt>::difference_type
count(InputIt first, InputIt last, const T& value) {
    typename iterator_traits<InputIt>::difference_type n = 0;
    for (; first != last; ++first) {
        if (*first == value) ++n;
    }
    return n;
}

template<class InputIt, class UnaryPredicate>
inline typename iterator_traits<InputIt>::difference_type
count_if(InputIt first, InputIt last, UnaryPredicate pred) {
    typename iterator_traits<InputIt>::difference_type n = 0;
    for (; first != last; ++first) {
        if (pred(*first)) ++n;
    }
    return n;
}

} // namespace prototype

#endif // PROTOTYPE_ALGORITHM_FIND_HPP
