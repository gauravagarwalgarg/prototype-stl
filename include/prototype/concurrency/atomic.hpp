///////////////////////////////////////////////////////////////////////////////
// include/prototype/concurrency/atomic.hpp
//
// PrototypeSTL - atomic<T>, atomic_flag, memory fences
// Uses GCC __atomic builtins for lock-free operations.
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ATOMIC_HPP
#define PROTOTYPE_ATOMIC_HPP

#include "../config.hpp"
#include "../type_traits/type_traits.hpp"

namespace prototype {

///////////////////////////////////////////////////////////////////////////////
// Memory ordering
///////////////////////////////////////////////////////////////////////////////
enum memory_order {
    memory_order_relaxed = __ATOMIC_RELAXED,
    memory_order_consume = __ATOMIC_CONSUME,
    memory_order_acquire = __ATOMIC_ACQUIRE,
    memory_order_release = __ATOMIC_RELEASE,
    memory_order_acq_rel = __ATOMIC_ACQ_REL,
    memory_order_seq_cst = __ATOMIC_SEQ_CST
};

///////////////////////////////////////////////////////////////////////////////
// Fences
///////////////////////////////////////////////////////////////////////////////
inline void atomic_thread_fence(memory_order order) {
    __atomic_thread_fence(order);
}

inline void atomic_signal_fence(memory_order order) {
    __atomic_signal_fence(order);
}

///////////////////////////////////////////////////////////////////////////////
// atomic_flag
///////////////////////////////////////////////////////////////////////////////
class atomic_flag {
public:
    atomic_flag() : flag_(0) {}

    bool test_and_set(memory_order order = memory_order_seq_cst) {
        return __atomic_test_and_set(&flag_, order);
    }

    void clear(memory_order order = memory_order_seq_cst) {
        __atomic_clear(&flag_, order);
    }

    // Non-copyable
    atomic_flag(const atomic_flag&) = delete;
    atomic_flag& operator=(const atomic_flag&) = delete;

private:
    unsigned char flag_;
};

///////////////////////////////////////////////////////////////////////////////
// atomic<T>
///////////////////////////////////////////////////////////////////////////////
template<class T>
class atomic {
public:
    atomic() : value_() {}
    explicit atomic(T desired) : value_(desired) {}

    // Non-copyable
    atomic(const atomic&) = delete;
    atomic& operator=(const atomic&) = delete;

    T load(memory_order order = memory_order_seq_cst) const {
        T result;
        __atomic_load(&value_, &result, order);
        return result;
    }

    void store(T desired, memory_order order = memory_order_seq_cst) {
        __atomic_store(&value_, &desired, order);
    }

    T exchange(T desired, memory_order order = memory_order_seq_cst) {
        T result;
        __atomic_exchange(&value_, &desired, &result, order);
        return result;
    }

    bool compare_exchange_strong(T& expected, T desired,
                                 memory_order success = memory_order_seq_cst,
                                 memory_order failure = memory_order_seq_cst) {
        return __atomic_compare_exchange(&value_, &expected, &desired,
                                         false, success, failure);
    }

    bool compare_exchange_weak(T& expected, T desired,
                               memory_order success = memory_order_seq_cst,
                               memory_order failure = memory_order_seq_cst) {
        return __atomic_compare_exchange(&value_, &expected, &desired,
                                         true, success, failure);
    }

    // Arithmetic operations (only for integral types)
    T fetch_add(T arg, memory_order order = memory_order_seq_cst) {
        return __atomic_fetch_add(&value_, arg, order);
    }

    T fetch_sub(T arg, memory_order order = memory_order_seq_cst) {
        return __atomic_fetch_sub(&value_, arg, order);
    }

    T fetch_and(T arg, memory_order order = memory_order_seq_cst) {
        return __atomic_fetch_and(&value_, arg, order);
    }

    T fetch_or(T arg, memory_order order = memory_order_seq_cst) {
        return __atomic_fetch_or(&value_, arg, order);
    }

    T fetch_xor(T arg, memory_order order = memory_order_seq_cst) {
        return __atomic_fetch_xor(&value_, arg, order);
    }

    // Operators
    T operator++() { return fetch_add(1) + 1; }
    T operator++(int) { return fetch_add(1); }
    T operator--() { return fetch_sub(1) - 1; }
    T operator--(int) { return fetch_sub(1); }
    T operator+=(T arg) { return fetch_add(arg) + arg; }
    T operator-=(T arg) { return fetch_sub(arg) - arg; }

    operator T() const { return load(); }

private:
    T value_;
};

} // namespace prototype

#endif // PROTOTYPE_ATOMIC_HPP
