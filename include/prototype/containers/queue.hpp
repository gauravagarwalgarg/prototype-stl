///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/queue.hpp
//
// PrototypeSTL - Queue and priority_queue container adaptors
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_QUEUE_HPP
#define PROTOTYPE_QUEUE_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"
#include "../algorithms/heap.hpp"

namespace prototype {

///////////////////////////////////////////////////////////////////////////////
// queue - FIFO adaptor with fixed capacity
///////////////////////////////////////////////////////////////////////////////
template<class T, size_t Capacity>
class queue {
public:
    typedef T         value_type;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef size_t    size_type;

    queue() : head_(0), size_(0) {}

    ~queue() { clear(); }

    reference front() { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return *ptr(head_); }
    const_reference front() const { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return *ptr(head_); }
    reference back() { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return *ptr(wrap(head_ + size_ - 1)); }
    const_reference back() const { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return *ptr(wrap(head_ + size_ - 1)); }

    bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }
    bool full() const { return size_ == Capacity; }

    void push(const T& value) {
        PROTOTYPE_ASSERT(size_ < Capacity);
        size_type idx = wrap(head_ + size_);
        ::new(static_cast<void*>(ptr(idx))) T(value);
        ++size_;
    }

    void pop() {
        PROTOTYPE_DEBUG_ASSERT(size_ > 0);
        ptr(head_)->~T();
        head_ = wrap(head_ + 1);
        --size_;
    }

    void clear() {
        while (size_ > 0) pop();
    }

private:
    size_type wrap(size_type i) const { return i % Capacity; }
    T* ptr(size_type i) { return reinterpret_cast<T*>(storage_) + i; }
    const T* ptr(size_type i) const { return reinterpret_cast<const T*>(storage_) + i; }

    PROTOTYPE_ALIGNAS(alignof(T)) char storage_[sizeof(T) * Capacity];
    size_type head_;
    size_type size_;
};

///////////////////////////////////////////////////////////////////////////////
// priority_queue - max-heap based priority queue
///////////////////////////////////////////////////////////////////////////////
template<class T, size_t Capacity>
class priority_queue {
public:
    typedef T         value_type;
    typedef const T&  const_reference;
    typedef size_t    size_type;

    priority_queue() : size_(0) {}

    ~priority_queue() {
        for (size_type i = 0; i < size_; ++i) ptr(i)->~T();
    }

    const_reference top() const { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return *ptr(0); }

    bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }

    void push(const T& value) {
        PROTOTYPE_ASSERT(size_ < Capacity);
        ::new(static_cast<void*>(ptr(size_))) T(value);
        ++size_;
        push_heap(ptr(0), ptr(0) + size_);
    }

    void pop() {
        PROTOTYPE_DEBUG_ASSERT(size_ > 0);
        pop_heap(ptr(0), ptr(0) + size_);
        --size_;
        ptr(size_)->~T();
    }

private:
    T* ptr(size_type i) { return reinterpret_cast<T*>(storage_) + i; }
    const T* ptr(size_type i) const { return reinterpret_cast<const T*>(storage_) + i; }

    PROTOTYPE_ALIGNAS(alignof(T)) char storage_[sizeof(T) * Capacity];
    size_type size_;
};

} // namespace prototype

#endif // PROTOTYPE_QUEUE_HPP
