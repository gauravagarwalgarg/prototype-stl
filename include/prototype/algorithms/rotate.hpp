///////////////////////////////////////////////////////////////////////////////
// include/prototype/algorithms/rotate.hpp
//
// PrototypeSTL - rotate, reverse
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALGORITHM_ROTATE_HPP
#define PROTOTYPE_ALGORITHM_ROTATE_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"

namespace prototype {

template<class BidirIt>
inline void reverse(BidirIt first, BidirIt last) {
    while (first != last) {
        --last;
        if (first == last) break;
        prototype::swap(*first, *last);
        ++first;
    }
}

template<class ForwardIt>
inline ForwardIt rotate(ForwardIt first, ForwardIt middle, ForwardIt last) {
    if (first == middle) return last;
    if (middle == last) return first;

    ForwardIt next = middle;
    ForwardIt ret = first;
    bool ret_set = false;

    while (first != next) {
        prototype::swap(*first, *next);
        ++first;
        ++next;
        if (!ret_set && first == middle) {
            ret = next;
            ret_set = true;
        }
        if (next == last) {
            next = middle;
        } else if (first == middle) {
            middle = next;
        }
    }
    if (!ret_set) ret = first;
    return ret;
}

} // namespace prototype

#endif // PROTOTYPE_ALGORITHM_ROTATE_HPP
