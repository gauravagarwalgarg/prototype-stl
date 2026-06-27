///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/ring_buffer.hpp
//
// PrototypeSTL - SPSC (single-producer single-consumer) ring buffer
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_RING_BUFFER_HPP
#define PROTOTYPE_RING_BUFFER_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"

namespace prototype {

template<class T, size_t Capacity>
class ring_buffer {
    // Capacity must be power of 2 for efficient wrapping
    static_assert((Capacity & (Capacity - 1)) == 0, "ring_buffer capacity must be power of 2");

public:
    ring_buffer() : head_(0), tail_(0) {}

    bool empty() const { return head_ == tail_; }
    bool full() const { return size() == Capacity; }
    size_t size() const { return tail_ - head_; }
    size_t capacity() const { return Capacity; }

    bool push(const T& value) {
        if (full()) return false;
        data_[tail_ & (Capacity - 1)] = value;
        ++tail_;
        return true;
    }

    bool push(T&& value) {
        if (full()) return false;
        data_[tail_ & (Capacity - 1)] = prototype::move(value);
        ++tail_;
        return true;
    }

    bool pop(T& out) {
        if (empty()) return false;
        out = prototype::move(data_[head_ & (Capacity - 1)]);
        ++head_;
        return true;
    }

    const T& front() const {
        PROTOTYPE_DEBUG_ASSERT(!empty());
        return data_[head_ & (Capacity - 1)];
    }

    void clear() { head_ = tail_ = 0; }

private:
    T      data_[Capacity];
    size_t head_;
    size_t tail_;
};

} // namespace prototype

#endif // PROTOTYPE_RING_BUFFER_HPP
