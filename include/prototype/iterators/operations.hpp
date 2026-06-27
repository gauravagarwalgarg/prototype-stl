///////////////////////////////////////////////////////////////////////////////
// include/prototype/iterators/operations.hpp
//
// PrototypeSTL - Iterator operations: advance, distance, next, prev
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ITERATOR_OPERATIONS_HPP
#define PROTOTYPE_ITERATOR_OPERATIONS_HPP

#include "../config.hpp"
#include "iterator_traits.hpp"

namespace prototype {

namespace detail {

template<class InputIt, class Distance>
inline void advance_impl(InputIt& it, Distance n, input_iterator_tag) {
    for (; n > 0; --n) ++it;
}

template<class BidirIt, class Distance>
inline void advance_impl(BidirIt& it, Distance n, bidirectional_iterator_tag) {
    if (n > 0) { for (; n > 0; --n) ++it; }
    else       { for (; n < 0; ++n) --it; }
}

template<class RandIt, class Distance>
inline void advance_impl(RandIt& it, Distance n, random_access_iterator_tag) {
    it += n;
}

template<class InputIt>
inline typename iterator_traits<InputIt>::difference_type
distance_impl(InputIt first, InputIt last, input_iterator_tag) {
    typename iterator_traits<InputIt>::difference_type d = 0;
    for (; first != last; ++first) ++d;
    return d;
}

template<class RandIt>
inline typename iterator_traits<RandIt>::difference_type
distance_impl(RandIt first, RandIt last, random_access_iterator_tag) {
    return last - first;
}

} // namespace detail

template<class InputIt, class Distance>
inline void advance(InputIt& it, Distance n) {
    detail::advance_impl(it, n, typename iterator_traits<InputIt>::iterator_category());
}

template<class InputIt>
inline typename iterator_traits<InputIt>::difference_type
distance(InputIt first, InputIt last) {
    return detail::distance_impl(first, last, typename iterator_traits<InputIt>::iterator_category());
}

template<class ForwardIt>
inline ForwardIt next(ForwardIt it, typename iterator_traits<ForwardIt>::difference_type n = 1) {
    advance(it, n);
    return it;
}

template<class BidirIt>
inline BidirIt prev(BidirIt it, typename iterator_traits<BidirIt>::difference_type n = 1) {
    advance(it, -n);
    return it;
}

} // namespace prototype

#endif // PROTOTYPE_ITERATOR_OPERATIONS_HPP
