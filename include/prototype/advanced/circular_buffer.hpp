///////////////////////////////////////////////////////////////////////////////
// include/prototype/advanced/circular_buffer.hpp
//
// PrototypeSTL - Fixed-capacity circular buffer (overwrites oldest on full)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ADVANCED_CIRCULAR_BUFFER_HPP
#define PROTOTYPE_ADVANCED_CIRCULAR_BUFFER_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"
#include "../containers/optional.hpp"

namespace prototype {

template<class T, size_t N>
class fixed_circular_buffer {
public:
    typedef T         value_type;
    typedef size_t    size_type;

    fixed_circular_buffer() : head_(0), tail_(0), size_(0) {}

    ~fixed_circular_buffer() {
        while (!empty()) {
            pop();
        }
    }

    /// Push value. Overwrites oldest if full.
    void push(const T& value) {
        if (full()) {
            // Overwrite oldest
            ptr(head_)->~T();
            head_ = (head_ + 1) % N;
            --size_;
        }
        ::new(static_cast<void*>(ptr(tail_))) T(value);
        tail_ = (tail_ + 1) % N;
        ++size_;
    }

    void push(T&& value) {
        if (full()) {
            ptr(head_)->~T();
            head_ = (head_ + 1) % N;
            --size_;
        }
        ::new(static_cast<void*>(ptr(tail_))) T(prototype::move(value));
        tail_ = (tail_ + 1) % N;
        ++size_;
    }

    /// Pop oldest element
    optional<T> pop() {
        if (empty()) return optional<T>();
        T val = prototype::move(*ptr(head_));
        ptr(head_)->~T();
        head_ = (head_ + 1) % N;
        --size_;
        return optional<T>(prototype::move(val));
    }

    /// View front without removing
    const T& peek() const {
        PROTOTYPE_ASSERT(!empty());
        return *ptr(head_);
    }

    T& peek() {
        PROTOTYPE_ASSERT(!empty());
        return *ptr(head_);
    }

    bool full() const { return size_ == N; }
    bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }
    constexpr size_type capacity() const { return N; }

private:
    T* ptr(size_t i) {
        return reinterpret_cast<T*>(storage_) + i;
    }
    const T* ptr(size_t i) const {
        return reinterpret_cast<const T*>(storage_) + i;
    }

    PROTOTYPE_ALIGNAS(alignof(T)) char storage_[sizeof(T) * N];
    size_t    head_;
    size_t    tail_;
    size_type size_;
};

} // namespace prototype

#endif // PROTOTYPE_ADVANCED_CIRCULAR_BUFFER_HPP
