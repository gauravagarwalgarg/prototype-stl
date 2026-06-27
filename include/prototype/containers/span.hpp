///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/span.hpp
//
// PrototypeSTL - Non-owning view over a contiguous range
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_SPAN_HPP
#define PROTOTYPE_SPAN_HPP

#include "../config.hpp"

namespace prototype {

template<class T>
class span {
public:
    typedef T              element_type;
    typedef T              value_type;
    typedef size_t         size_type;
    typedef ptrdiff_t      difference_type;
    typedef T*             pointer;
    typedef const T*       const_pointer;
    typedef T&             reference;
    typedef const T&       const_reference;
    typedef T*             iterator;
    typedef const T*       const_iterator;

    span() : data_(nullptr), size_(0) {}
    span(T* ptr, size_type count) : data_(ptr), size_(count) {}
    span(T* first, T* last) : data_(first), size_(static_cast<size_type>(last - first)) {}

    template<size_t N>
    span(T (&arr)[N]) : data_(arr), size_(N) {}

    // Element access
    reference operator[](size_type i) const { PROTOTYPE_DEBUG_ASSERT(i < size_); return data_[i]; }
    reference front() const { return data_[0]; }
    reference back() const { return data_[size_ - 1]; }
    pointer data() const { return data_; }

    // Iterators
    iterator begin() const { return data_; }
    iterator end() const { return data_ + size_; }

    // Capacity
    size_type size() const { return size_; }
    size_type size_bytes() const { return size_ * sizeof(T); }
    bool empty() const { return size_ == 0; }

    // Subviews
    span first(size_type count) const {
        PROTOTYPE_DEBUG_ASSERT(count <= size_);
        return span(data_, count);
    }

    span last(size_type count) const {
        PROTOTYPE_DEBUG_ASSERT(count <= size_);
        return span(data_ + (size_ - count), count);
    }

    span subspan(size_type offset, size_type count = static_cast<size_type>(-1)) const {
        PROTOTYPE_DEBUG_ASSERT(offset <= size_);
        size_type n = (count == static_cast<size_type>(-1)) ? size_ - offset : count;
        PROTOTYPE_DEBUG_ASSERT(offset + n <= size_);
        return span(data_ + offset, n);
    }

private:
    T*        data_;
    size_type size_;
};

} // namespace prototype

#endif // PROTOTYPE_SPAN_HPP
