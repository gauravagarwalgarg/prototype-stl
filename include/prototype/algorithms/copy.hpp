///////////////////////////////////////////////////////////////////////////////
// include/prototype/algorithms/copy.hpp
//
// PrototypeSTL - copy, copy_backward, copy_n, move_range
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALGORITHM_COPY_HPP
#define PROTOTYPE_ALGORITHM_COPY_HPP

#include "../config.hpp"
#include "../type_traits/type_traits.hpp"
#include "../utility/utility.hpp"
#include "../iterators/iterator_traits.hpp"

namespace prototype {

template<class InputIt, class OutputIt>
inline OutputIt copy(InputIt first, InputIt last, OutputIt d_first) {
    for (; first != last; ++first, ++d_first) {
        *d_first = *first;
    }
    return d_first;
}

template<class BidirIt1, class BidirIt2>
inline BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last) {
    while (first != last) {
        *(--d_last) = *(--last);
    }
    return d_last;
}

template<class InputIt, class Size, class OutputIt>
inline OutputIt copy_n(InputIt first, Size count, OutputIt result) {
    for (Size i = 0; i < count; ++i, ++first, ++result) {
        *result = *first;
    }
    return result;
}

template<class InputIt, class OutputIt>
inline OutputIt move_range(InputIt first, InputIt last, OutputIt d_first) {
    for (; first != last; ++first, ++d_first) {
        *d_first = prototype::move(*first);
    }
    return d_first;
}

template<class BidirIt1, class BidirIt2>
inline BidirIt2 move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last) {
    while (first != last) {
        *(--d_last) = prototype::move(*(--last));
    }
    return d_last;
}

} // namespace prototype

#endif // PROTOTYPE_ALGORITHM_COPY_HPP
