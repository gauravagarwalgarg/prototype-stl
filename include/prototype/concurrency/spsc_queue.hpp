///////////////////////////////////////////////////////////////////////////////
// include/prototype/concurrency/spsc_queue.hpp
//
// PrototypeSTL - Lock-free single-producer single-consumer queue
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_SPSC_QUEUE_HPP
#define PROTOTYPE_SPSC_QUEUE_HPP

#include "../config.hpp"
#include "../type_traits/type_traits.hpp"
#include "../utility/utility.hpp"
#include "atomic.hpp"

namespace prototype {

template<class T, size_t Capacity>
class spsc_queue {
    static_assert((Capacity & (Capacity - 1)) == 0, "spsc_queue capacity must be power of 2");

public:
    spsc_queue() : head_(0), tail_(0) {}

    bool push(const T& value) {
        size_t tail = tail_.load(memory_order_relaxed);
        size_t next = (tail + 1) & (Capacity - 1);
        if (next == head_.load(memory_order_acquire)) {
            return false; // full
        }
        data_[tail] = value;
        tail_.store(next, memory_order_release);
        return true;
    }

    bool push(T&& value) {
        size_t tail = tail_.load(memory_order_relaxed);
        size_t next = (tail + 1) & (Capacity - 1);
        if (next == head_.load(memory_order_acquire)) {
            return false; // full
        }
        data_[tail] = prototype::move(value);
        tail_.store(next, memory_order_release);
        return true;
    }

    bool pop(T& out) {
        size_t head = head_.load(memory_order_relaxed);
        if (head == tail_.load(memory_order_acquire)) {
            return false; // empty
        }
        out = prototype::move(data_[head]);
        head_.store((head + 1) & (Capacity - 1), memory_order_release);
        return true;
    }

    bool empty() const {
        return head_.load(memory_order_acquire) == tail_.load(memory_order_acquire);
    }

    size_t size() const {
        size_t h = head_.load(memory_order_acquire);
        size_t t = tail_.load(memory_order_acquire);
        return (t - h) & (Capacity - 1);
    }

private:
    T data_[Capacity];
    PROTOTYPE_ALIGNAS(PROTOTYPE_CACHE_LINE) atomic<size_t> head_;
    PROTOTYPE_ALIGNAS(PROTOTYPE_CACHE_LINE) atomic<size_t> tail_;
};

} // namespace prototype

#endif // PROTOTYPE_SPSC_QUEUE_HPP
