///////////////////////////////////////////////////////////////////////////////
// include/prototype/algorithms/numeric.hpp
//
// PrototypeSTL - accumulate, iota, gcd, lcm, clamp
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALGORITHM_NUMERIC_HPP
#define PROTOTYPE_ALGORITHM_NUMERIC_HPP

#include "../config.hpp"

namespace prototype {

template<class InputIt, class T>
inline T accumulate(InputIt first, InputIt last, T init) {
    for (; first != last; ++first) {
        init = init + *first;
    }
    return init;
}

template<class InputIt, class T, class BinaryOp>
inline T accumulate(InputIt first, InputIt last, T init, BinaryOp op) {
    for (; first != last; ++first) {
        init = op(init, *first);
    }
    return init;
}

template<class ForwardIt, class T>
inline void iota(ForwardIt first, ForwardIt last, T value) {
    for (; first != last; ++first, ++value) {
        *first = value;
    }
}

template<class T>
inline T gcd(T a, T b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        T t = b;
        b = a % b;
        a = t;
    }
    return a;
}

template<class T>
inline T lcm(T a, T b) {
    if (a == 0 || b == 0) return 0;
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    return (a / gcd(a, b)) * b;
}

template<class T>
inline const T& clamp(const T& v, const T& lo, const T& hi) {
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

template<class T, class Compare>
inline const T& clamp(const T& v, const T& lo, const T& hi, Compare comp) {
    return comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}

template<class InputIt1, class InputIt2, class T>
inline T inner_product(InputIt1 first1, InputIt1 last1, InputIt2 first2, T init) {
    for (; first1 != last1; ++first1, ++first2) {
        init = init + (*first1) * (*first2);
    }
    return init;
}

} // namespace prototype

#endif // PROTOTYPE_ALGORITHM_NUMERIC_HPP
