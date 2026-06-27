///////////////////////////////////////////////////////////////////////////////
// include/prototype/utility/utility.hpp
//
// PrototypeSTL - Utility components: move, forward, swap, pair, min, max
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_UTILITY_HPP
#define PROTOTYPE_UTILITY_HPP

#include "../config.hpp"
#include "../type_traits/type_traits.hpp"

namespace prototype {

///////////////////////////////////////////////////////////////////////////////
// move
///////////////////////////////////////////////////////////////////////////////
template<class T>
inline typename remove_reference<T>::type&& move(T&& t) {
    return static_cast<typename remove_reference<T>::type&&>(t);
}

///////////////////////////////////////////////////////////////////////////////
// forward
///////////////////////////////////////////////////////////////////////////////
template<class T>
inline T&& forward(typename remove_reference<T>::type& t) {
    return static_cast<T&&>(t);
}

template<class T>
inline T&& forward(typename remove_reference<T>::type&& t) {
    return static_cast<T&&>(t);
}

///////////////////////////////////////////////////////////////////////////////
// swap
///////////////////////////////////////////////////////////////////////////////
template<class T>
inline void swap(T& a, T& b) {
    T tmp = move(a);
    a = move(b);
    b = move(tmp);
}

template<class T, size_t N>
inline void swap(T (&a)[N], T (&b)[N]) {
    for (size_t i = 0; i < N; ++i) {
        swap(a[i], b[i]);
    }
}

///////////////////////////////////////////////////////////////////////////////
// min, max
///////////////////////////////////////////////////////////////////////////////
template<class T>
inline const T& min(const T& a, const T& b) {
    return (b < a) ? b : a;
}

template<class T>
inline const T& max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

template<class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp) {
    return comp(b, a) ? b : a;
}

template<class T, class Compare>
inline const T& max(const T& a, const T& b, Compare comp) {
    return comp(a, b) ? b : a;
}

///////////////////////////////////////////////////////////////////////////////
// pair
///////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
struct pair {
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;

    pair() : first(), second() {}
    pair(const T1& a, const T2& b) : first(a), second(b) {}

    template<class U1, class U2>
    pair(const pair<U1, U2>& other) : first(other.first), second(other.second) {}

    pair(T1&& a, T2&& b)
        : first(prototype::move(a)), second(prototype::move(b)) {}
};

template<class T1, class T2>
inline bool operator==(const pair<T1,T2>& a, const pair<T1,T2>& b) {
    return a.first == b.first && a.second == b.second;
}

template<class T1, class T2>
inline bool operator!=(const pair<T1,T2>& a, const pair<T1,T2>& b) {
    return !(a == b);
}

template<class T1, class T2>
inline bool operator<(const pair<T1,T2>& a, const pair<T1,T2>& b) {
    if (a.first < b.first) return true;
    if (b.first < a.first) return false;
    return a.second < b.second;
}

template<class T1, class T2>
inline pair<T1, T2> make_pair(T1 a, T2 b) {
    return pair<T1, T2>(a, b);
}

///////////////////////////////////////////////////////////////////////////////
// integer_sequence (C++11 compatible)
///////////////////////////////////////////////////////////////////////////////
template<class T, T... Ints>
struct integer_sequence {
    static constexpr size_t size() { return sizeof...(Ints); }
};

template<size_t... Ints>
using index_sequence = integer_sequence<size_t, Ints...>;

} // namespace prototype

#endif // PROTOTYPE_UTILITY_HPP
