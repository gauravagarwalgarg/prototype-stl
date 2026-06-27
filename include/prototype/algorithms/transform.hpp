///////////////////////////////////////////////////////////////////////////////
// include/prototype/algorithms/transform.hpp
//
// PrototypeSTL - transform, for_each
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALGORITHM_TRANSFORM_HPP
#define PROTOTYPE_ALGORITHM_TRANSFORM_HPP

#include "../config.hpp"

namespace prototype {

template<class InputIt, class OutputIt, class UnaryOp>
inline OutputIt transform(InputIt first, InputIt last, OutputIt d_first, UnaryOp op) {
    for (; first != last; ++first, ++d_first) {
        *d_first = op(*first);
    }
    return d_first;
}

template<class InputIt1, class InputIt2, class OutputIt, class BinaryOp>
inline OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                          OutputIt d_first, BinaryOp op) {
    for (; first1 != last1; ++first1, ++first2, ++d_first) {
        *d_first = op(*first1, *first2);
    }
    return d_first;
}

template<class InputIt, class UnaryFunction>
inline UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f) {
    for (; first != last; ++first) {
        f(*first);
    }
    return f;
}

} // namespace prototype

#endif // PROTOTYPE_ALGORITHM_TRANSFORM_HPP
