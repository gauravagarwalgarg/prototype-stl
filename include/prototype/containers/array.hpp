///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/array.hpp
//
// PrototypeSTL - Fixed-size array container
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ARRAY_HPP
#define PROTOTYPE_ARRAY_HPP

#include "../config.hpp"
#include "../iterators/iterator_traits.hpp"
#include "../iterators/reverse_iterator.hpp"
#include "../algorithms/fill.hpp"

namespace prototype {

template<class T, size_t N>
struct array {
    typedef T                 value_type;
    typedef T*                iterator;
    typedef const T*          const_iterator;
    typedef T&                reference;
    typedef const T&          const_reference;
    typedef size_t            size_type;
    typedef ptrdiff_t         difference_type;
    typedef reverse_iterator<const_iterator> const_reverse_iterator;
    typedef reverse_iterator<iterator>       reverse_iterator_type;

    // Data member - aggregate initialization
    T data_[N];

    // Element access
    reference operator[](size_type i) { PROTOTYPE_DEBUG_ASSERT(i < N); return data_[i]; }
    const_reference operator[](size_type i) const { PROTOTYPE_DEBUG_ASSERT(i < N); return data_[i]; }
    reference at(size_type i) { PROTOTYPE_ASSERT(i < N); return data_[i]; }
    const_reference at(size_type i) const { PROTOTYPE_ASSERT(i < N); return data_[i]; }
    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }
    reference back() { return data_[N - 1]; }
    const_reference back() const { return data_[N - 1]; }
    T* data() { return data_; }
    const T* data() const { return data_; }

    // Iterators
    iterator begin() { return data_; }
    iterator end() { return data_ + N; }
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ + N; }
    const_iterator cbegin() const { return data_; }
    const_iterator cend() const { return data_ + N; }

    // Capacity
    constexpr size_type size() const { return N; }
    constexpr size_type max_size() const { return N; }
    constexpr bool empty() const { return N == 0; }

    // Operations
    void fill(const T& value) {
        prototype::fill(data_, data_ + N, value);
    }

    void swap(array& other) {
        for (size_type i = 0; i < N; ++i) {
            T tmp = data_[i];
            data_[i] = other.data_[i];
            other.data_[i] = tmp;
        }
    }
};

template<class T, size_t N>
inline bool operator==(const array<T,N>& a, const array<T,N>& b) {
    for (size_t i = 0; i < N; ++i) {
        if (!(a[i] == b[i])) return false;
    }
    return true;
}

template<class T, size_t N>
inline bool operator!=(const array<T,N>& a, const array<T,N>& b) {
    return !(a == b);
}

} // namespace prototype

#endif // PROTOTYPE_ARRAY_HPP
