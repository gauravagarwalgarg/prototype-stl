///////////////////////////////////////////////////////////////////////////////
// include/prototype/concurrency/spinlock.hpp
//
// PrototypeSTL - Spinlock and scoped_lock
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_SPINLOCK_HPP
#define PROTOTYPE_SPINLOCK_HPP

#include "../config.hpp"
#include "atomic.hpp"

namespace prototype {

class spinlock {
public:
    spinlock() {}

    void lock() {
        while (flag_.test_and_set(memory_order_acquire)) {
            // spin - could add pause/yield hint here
        }
    }

    bool try_lock() {
        return !flag_.test_and_set(memory_order_acquire);
    }

    void unlock() {
        flag_.clear(memory_order_release);
    }

    // Non-copyable
    spinlock(const spinlock&) = delete;
    spinlock& operator=(const spinlock&) = delete;

private:
    atomic_flag flag_;
};

///////////////////////////////////////////////////////////////////////////////
// scoped_lock - RAII lock guard
///////////////////////////////////////////////////////////////////////////////
template<class Mutex>
class scoped_lock {
public:
    explicit scoped_lock(Mutex& m) : mutex_(m) { mutex_.lock(); }
    ~scoped_lock() { mutex_.unlock(); }

    scoped_lock(const scoped_lock&) = delete;
    scoped_lock& operator=(const scoped_lock&) = delete;

private:
    Mutex& mutex_;
};

} // namespace prototype

#endif // PROTOTYPE_SPINLOCK_HPP
