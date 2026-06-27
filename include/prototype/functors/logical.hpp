///////////////////////////////////////////////////////////////////////////////
// include/prototype/functors/logical.hpp
//
// PrototypeSTL - Logical function objects: logical_and, logical_or, logical_not
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_FUNCTORS_LOGICAL_HPP
#define PROTOTYPE_FUNCTORS_LOGICAL_HPP

#include "../config.hpp"

namespace prototype {

template<class T = void>
struct logical_and {
    bool operator()(const T& a, const T& b) const { return a && b; }
};

template<>
struct logical_and<void> {
    template<class T, class U>
    bool operator()(const T& a, const U& b) const { return a && b; }
};

template<class T = void>
struct logical_or {
    bool operator()(const T& a, const T& b) const { return a || b; }
};

template<>
struct logical_or<void> {
    template<class T, class U>
    bool operator()(const T& a, const U& b) const { return a || b; }
};

template<class T = void>
struct logical_not {
    bool operator()(const T& a) const { return !a; }
};

template<>
struct logical_not<void> {
    template<class T>
    bool operator()(const T& a) const { return !a; }
};

} // namespace prototype

#endif // PROTOTYPE_FUNCTORS_LOGICAL_HPP
