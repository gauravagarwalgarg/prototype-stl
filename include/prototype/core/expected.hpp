///////////////////////////////////////////////////////////////////////////////
// include/prototype/core/expected.hpp
//
// PrototypeSTL - Error-or-value return type (like std::expected / Rust Result)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_CORE_EXPECTED_HPP
#define PROTOTYPE_CORE_EXPECTED_HPP

#include "../config.hpp"
#include "../type_traits/type_traits.hpp"
#include "../utility/utility.hpp"

namespace prototype {

/// Tag type for constructing the error case
struct unexpect_t { explicit unexpect_t() {} };
static const unexpect_t unexpect = unexpect_t();

///////////////////////////////////////////////////////////////////////////////
// expected<T, E>
///////////////////////////////////////////////////////////////////////////////
template<class T, class E>
class expected {
public:
    typedef T value_type;
    typedef E error_type;

    // Construct with value (success)
    expected(const T& val) : has_val_(true) {
        ::new(static_cast<void*>(val_buf())) T(val);
    }

    expected(T&& val) : has_val_(true) {
        ::new(static_cast<void*>(val_buf())) T(prototype::move(val));
    }

    // Construct with error
    expected(unexpect_t, const E& err) : has_val_(false) {
        ::new(static_cast<void*>(err_buf())) E(err);
    }

    expected(unexpect_t, E&& err) : has_val_(false) {
        ::new(static_cast<void*>(err_buf())) E(prototype::move(err));
    }

    expected(const expected& other) : has_val_(other.has_val_) {
        if (has_val_) {
            ::new(static_cast<void*>(val_buf())) T(*other.val_ptr());
        } else {
            ::new(static_cast<void*>(err_buf())) E(*other.err_ptr());
        }
    }

    expected(expected&& other) : has_val_(other.has_val_) {
        if (has_val_) {
            ::new(static_cast<void*>(val_buf())) T(prototype::move(*other.val_ptr()));
        } else {
            ::new(static_cast<void*>(err_buf())) E(prototype::move(*other.err_ptr()));
        }
    }

    ~expected() { destroy(); }

    expected& operator=(const expected& other) {
        if (this != &other) {
            destroy();
            has_val_ = other.has_val_;
            if (has_val_) {
                ::new(static_cast<void*>(val_buf())) T(*other.val_ptr());
            } else {
                ::new(static_cast<void*>(err_buf())) E(*other.err_ptr());
            }
        }
        return *this;
    }

    expected& operator=(expected&& other) {
        if (this != &other) {
            destroy();
            has_val_ = other.has_val_;
            if (has_val_) {
                ::new(static_cast<void*>(val_buf())) T(prototype::move(*other.val_ptr()));
            } else {
                ::new(static_cast<void*>(err_buf())) E(prototype::move(*other.err_ptr()));
            }
        }
        return *this;
    }

    // Observers
    bool has_value() const { return has_val_; }
    explicit operator bool() const { return has_val_; }

    T& value() {
        PROTOTYPE_ASSERT(has_val_);
        return *val_ptr();
    }

    const T& value() const {
        PROTOTYPE_ASSERT(has_val_);
        return *val_ptr();
    }

    E& error() {
        PROTOTYPE_ASSERT(!has_val_);
        return *err_ptr();
    }

    const E& error() const {
        PROTOTYPE_ASSERT(!has_val_);
        return *err_ptr();
    }

    T& operator*() { return value(); }
    const T& operator*() const { return value(); }
    T* operator->() { return val_ptr(); }
    const T* operator->() const { return val_ptr(); }

    T value_or(const T& default_val) const {
        return has_val_ ? *val_ptr() : default_val;
    }

private:
    void destroy() {
        if (has_val_) {
            val_ptr()->~T();
        } else {
            err_ptr()->~E();
        }
    }

    void* val_buf() { return storage_; }
    const void* val_buf() const { return storage_; }
    void* err_buf() { return storage_; }
    const void* err_buf() const { return storage_; }

    T* val_ptr() { return static_cast<T*>(val_buf()); }
    const T* val_ptr() const { return static_cast<const T*>(val_buf()); }
    E* err_ptr() { return static_cast<E*>(err_buf()); }
    const E* err_ptr() const { return static_cast<const E*>(err_buf()); }

    // Storage large enough for either T or E
    static const size_t STORAGE_SIZE = sizeof(T) > sizeof(E) ? sizeof(T) : sizeof(E);
    static const size_t STORAGE_ALIGN = alignof(T) > alignof(E) ? alignof(T) : alignof(E);

    PROTOTYPE_ALIGNAS(STORAGE_ALIGN) char storage_[STORAGE_SIZE];
    bool has_val_;
};

/// Helper to create an expected with an error value
template<class T, class E>
expected<T, E> make_unexpected(const E& err) {
    return expected<T, E>(unexpect, err);
}

} // namespace prototype

#endif // PROTOTYPE_CORE_EXPECTED_HPP
