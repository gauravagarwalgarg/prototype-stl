///////////////////////////////////////////////////////////////////////////////
// include/prototype/functors/arithmetic.hpp
//
// PrototypeSTL - Arithmetic function objects: plus, minus, multiplies, etc.
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_FUNCTORS_ARITHMETIC_HPP
#define PROTOTYPE_FUNCTORS_ARITHMETIC_HPP

#include "../config.hpp"

namespace prototype {

template<class T = void>
struct plus {
    T operator()(const T& a, const T& b) const { return a + b; }
};

template<>
struct plus<void> {
    template<class T, class U>
    auto operator()(const T& a, const U& b) const -> decltype(a + b) { return a + b; }
};

template<class T = void>
struct minus {
    T operator()(const T& a, const T& b) const { return a - b; }
};

template<>
struct minus<void> {
    template<class T, class U>
    auto operator()(const T& a, const U& b) const -> decltype(a - b) { return a - b; }
};

template<class T = void>
struct multiplies {
    T operator()(const T& a, const T& b) const { return a * b; }
};

template<>
struct multiplies<void> {
    template<class T, class U>
    auto operator()(const T& a, const U& b) const -> decltype(a * b) { return a * b; }
};

template<class T = void>
struct divides {
    T operator()(const T& a, const T& b) const { return a / b; }
};

template<>
struct divides<void> {
    template<class T, class U>
    auto operator()(const T& a, const U& b) const -> decltype(a / b) { return a / b; }
};

template<class T = void>
struct modulus {
    T operator()(const T& a, const T& b) const { return a % b; }
};

template<>
struct modulus<void> {
    template<class T, class U>
    auto operator()(const T& a, const U& b) const -> decltype(a % b) { return a % b; }
};

template<class T = void>
struct negate {
    T operator()(const T& a) const { return -a; }
};

template<>
struct negate<void> {
    template<class T>
    auto operator()(const T& a) const -> decltype(-a) { return -a; }
};

} // namespace prototype

#endif // PROTOTYPE_FUNCTORS_ARITHMETIC_HPP
