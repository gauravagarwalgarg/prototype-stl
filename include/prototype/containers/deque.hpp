///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/deque.hpp
//
// PrototypeSTL - fixed_deque (circular buffer based double-ended queue)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_DEQUE_HPP
#define PROTOTYPE_DEQUE_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"

namespace prototype {

template<class T, size_t Capacity>
class fixed_deque {
public:
    fixed_deque() : head_(0), size_(0) {}

    ~fixed_deque() {
        clear();
    }

    // Element access
    T& front() { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return *ptr(head_); }
    const T& front() const { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return *ptr(head_); }
    T& back() { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return *ptr(wrap(head_ + size_ - 1)); }
    const T& back() const { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return *ptr(wrap(head_ + size_ - 1)); }

    T& operator[](size_t i) { PROTOTYPE_DEBUG_ASSERT(i < size_); return *ptr(wrap(head_ + i)); }
    const T& operator[](size_t i) const { PROTOTYPE_DEBUG_ASSERT(i < size_); return *ptr(wrap(head_ + i)); }

    // Capacity
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    size_t capacity() const { return Capacity; }
    bool full() const { return size_ == Capacity; }

    // Modifiers
    void push_back(const T& value) {
        PROTOTYPE_ASSERT(size_ < Capacity);
        size_t idx = wrap(head_ + size_);
        ::new(static_cast<void*>(ptr(idx))) T(value);
        ++size_;
    }

    void push_front(const T& value) {
        PROTOTYPE_ASSERT(size_ < Capacity);
        head_ = wrap(head_ + Capacity - 1);
        ::new(static_cast<void*>(ptr(head_))) T(value);
        ++size_;
    }

    void pop_back() {
        PROTOTYPE_DEBUG_ASSERT(size_ > 0);
        --size_;
        ptr(wrap(head_ + size_))->~T();
    }

    void pop_front() {
        PROTOTYPE_DEBUG_ASSERT(size_ > 0);
        ptr(head_)->~T();
        head_ = wrap(head_ + 1);
        --size_;
    }

    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            ptr(wrap(head_ + i))->~T();
        }
        size_ = 0;
        head_ = 0;
    }

private:
    size_t wrap(size_t i) const { return i % Capacity; }
    T* ptr(size_t i) { return reinterpret_cast<T*>(storage_) + i; }
    const T* ptr(size_t i) const { return reinterpret_cast<const T*>(storage_) + i; }

    PROTOTYPE_ALIGNAS(alignof(T)) char storage_[sizeof(T) * Capacity];
    size_t head_;
    size_t size_;
};

} // namespace prototype

#endif // PROTOTYPE_DEQUE_HPP
