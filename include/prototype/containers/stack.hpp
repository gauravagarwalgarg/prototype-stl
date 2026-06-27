///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/stack.hpp
//
// PrototypeSTL - Stack container adaptor (fixed-capacity)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_STACK_HPP
#define PROTOTYPE_STACK_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"

namespace prototype {

template<class T, size_t Capacity>
class stack {
public:
    typedef T         value_type;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef size_t    size_type;

    stack() : size_(0) {}

    ~stack() {
        clear();
    }

    // Element access
    reference top() { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return *ptr(size_ - 1); }
    const_reference top() const { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return *ptr(size_ - 1); }

    // Capacity
    bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }
    bool full() const { return size_ == Capacity; }

    // Modifiers
    void push(const T& value) {
        PROTOTYPE_ASSERT(size_ < Capacity);
        ::new(static_cast<void*>(ptr(size_))) T(value);
        ++size_;
    }

    void push(T&& value) {
        PROTOTYPE_ASSERT(size_ < Capacity);
        ::new(static_cast<void*>(ptr(size_))) T(prototype::move(value));
        ++size_;
    }

    void pop() {
        PROTOTYPE_DEBUG_ASSERT(size_ > 0);
        --size_;
        ptr(size_)->~T();
    }

    void clear() {
        while (size_ > 0) pop();
    }

private:
    T* ptr(size_type i) { return reinterpret_cast<T*>(storage_) + i; }
    const T* ptr(size_type i) const { return reinterpret_cast<const T*>(storage_) + i; }

    PROTOTYPE_ALIGNAS(alignof(T)) char storage_[sizeof(T) * Capacity];
    size_type size_;
};

} // namespace prototype

#endif // PROTOTYPE_STACK_HPP
