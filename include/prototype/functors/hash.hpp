///////////////////////////////////////////////////////////////////////////////
// include/prototype/functors/hash.hpp
//
// PrototypeSTL - hash<T> specializations for built-in types
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_FUNCTORS_HASH_HPP
#define PROTOTYPE_FUNCTORS_HASH_HPP

#include "../config.hpp"

namespace prototype {

template<class T>
struct hash;

// Generic byte-based hash (FNV-1a)
namespace detail {
    inline size_t fnv1a(const void* data, size_t len) {
        const unsigned char* bytes = static_cast<const unsigned char*>(data);
        size_t h = 14695981039346656037ULL;
        for (size_t i = 0; i < len; ++i) {
            h ^= bytes[i];
            h *= 1099511628211ULL;
        }
        return h;
    }
}

// Integer types
template<> struct hash<bool> {
    size_t operator()(bool v) const { return v ? 1 : 0; }
};

template<> struct hash<char> {
    size_t operator()(char v) const { return static_cast<size_t>(v); }
};

template<> struct hash<signed char> {
    size_t operator()(signed char v) const { return static_cast<size_t>(v); }
};

template<> struct hash<unsigned char> {
    size_t operator()(unsigned char v) const { return static_cast<size_t>(v); }
};

template<> struct hash<short> {
    size_t operator()(short v) const { return static_cast<size_t>(v); }
};

template<> struct hash<unsigned short> {
    size_t operator()(unsigned short v) const { return static_cast<size_t>(v); }
};

template<> struct hash<int> {
    size_t operator()(int v) const { return static_cast<size_t>(v); }
};

template<> struct hash<unsigned int> {
    size_t operator()(unsigned int v) const { return static_cast<size_t>(v); }
};

template<> struct hash<long> {
    size_t operator()(long v) const { return static_cast<size_t>(v); }
};

template<> struct hash<unsigned long> {
    size_t operator()(unsigned long v) const { return v; }
};

template<> struct hash<long long> {
    size_t operator()(long long v) const { return static_cast<size_t>(v); }
};

template<> struct hash<unsigned long long> {
    size_t operator()(unsigned long long v) const { return static_cast<size_t>(v); }
};

// Pointer types
template<class T>
struct hash<T*> {
    size_t operator()(T* v) const {
        return reinterpret_cast<size_t>(v);
    }
};

// Floating point
template<> struct hash<float> {
    size_t operator()(float v) const {
        if (v == 0.0f) return 0;
        return detail::fnv1a(&v, sizeof(v));
    }
};

template<> struct hash<double> {
    size_t operator()(double v) const {
        if (v == 0.0) return 0;
        return detail::fnv1a(&v, sizeof(v));
    }
};

} // namespace prototype

#endif // PROTOTYPE_FUNCTORS_HASH_HPP
