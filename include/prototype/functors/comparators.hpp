///////////////////////////////////////////////////////////////////////////////
// include/prototype/functors/comparators.hpp
//
// PrototypeSTL - Comparison function objects: less, greater, equal_to, etc.
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_FUNCTORS_COMPARATORS_HPP
#define PROTOTYPE_FUNCTORS_COMPARATORS_HPP

#include "../config.hpp"

namespace prototype {

template<class T = void>
struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template<>
struct less<void> {
    template<class T, class U>
    bool operator()(const T& a, const U& b) const { return a < b; }
};

template<class T = void>
struct greater {
    bool operator()(const T& a, const T& b) const { return a > b; }
};

template<>
struct greater<void> {
    template<class T, class U>
    bool operator()(const T& a, const U& b) const { return a > b; }
};

template<class T = void>
struct less_equal {
    bool operator()(const T& a, const T& b) const { return a <= b; }
};

template<>
struct less_equal<void> {
    template<class T, class U>
    bool operator()(const T& a, const U& b) const { return a <= b; }
};

template<class T = void>
struct greater_equal {
    bool operator()(const T& a, const T& b) const { return a >= b; }
};

template<>
struct greater_equal<void> {
    template<class T, class U>
    bool operator()(const T& a, const U& b) const { return a >= b; }
};

template<class T = void>
struct equal_to {
    bool operator()(const T& a, const T& b) const { return a == b; }
};

template<>
struct equal_to<void> {
    template<class T, class U>
    bool operator()(const T& a, const U& b) const { return a == b; }
};

template<class T = void>
struct not_equal_to {
    bool operator()(const T& a, const T& b) const { return a != b; }
};

template<>
struct not_equal_to<void> {
    template<class T, class U>
    bool operator()(const T& a, const U& b) const { return a != b; }
};

} // namespace prototype

#endif // PROTOTYPE_FUNCTORS_COMPARATORS_HPP
