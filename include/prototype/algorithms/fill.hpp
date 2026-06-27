///////////////////////////////////////////////////////////////////////////////
// include/prototype/algorithms/fill.hpp
//
// PrototypeSTL - fill, fill_n
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALGORITHM_FILL_HPP
#define PROTOTYPE_ALGORITHM_FILL_HPP

#include "../config.hpp"

namespace prototype {

template<class ForwardIt, class T>
inline void fill(ForwardIt first, ForwardIt last, const T& value) {
    for (; first != last; ++first) {
        *first = value;
    }
}

template<class OutputIt, class Size, class T>
inline OutputIt fill_n(OutputIt first, Size count, const T& value) {
    for (Size i = 0; i < count; ++i, ++first) {
        *first = value;
    }
    return first;
}

} // namespace prototype

#endif // PROTOTYPE_ALGORITHM_FILL_HPP
