///////////////////////////////////////////////////////////////////////////////
// include/prototype/core/any.hpp
//
// PrototypeSTL - Type-erased container with fixed buffer (no heap, no RTTI)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_CORE_ANY_HPP
#define PROTOTYPE_CORE_ANY_HPP

#include "../config.hpp"
#include "../type_traits/type_traits.hpp"
#include "../utility/utility.hpp"

namespace prototype {

/// Type tag for identification without RTTI
typedef const void* type_id_t;

namespace detail {
    template<class T>
    struct type_tag {
        static const char id;
    };
    template<class T>
    const char type_tag<T>::id = 0;

    template<class T>
    inline type_id_t get_type_id() {
        return &type_tag<T>::id;
    }
} // namespace detail

///////////////////////////////////////////////////////////////////////////////
// fixed_any<BufferSize>
///////////////////////////////////////////////////////////////////////////////
template<size_t BufferSize = 64>
class fixed_any {
public:
    fixed_any() : ops_(nullptr), type_id_(nullptr) {}

    template<class T>
    fixed_any(const T& value) : ops_(nullptr), type_id_(nullptr) {
        // Static assert that T fits
        // Using a compile-time check via a helper
        construct<T>(value);
    }

    fixed_any(const fixed_any& other) : ops_(nullptr), type_id_(nullptr) {
        if (other.has_value()) {
            other.ops_->copy(buffer_, other.buffer_);
            ops_ = other.ops_;
            type_id_ = other.type_id_;
        }
    }

    fixed_any(fixed_any&& other) : ops_(nullptr), type_id_(nullptr) {
        if (other.has_value()) {
            other.ops_->move(buffer_, other.buffer_);
            ops_ = other.ops_;
            type_id_ = other.type_id_;
            other.reset();
        }
    }

    ~fixed_any() { reset(); }

    fixed_any& operator=(const fixed_any& other) {
        if (this != &other) {
            reset();
            if (other.has_value()) {
                other.ops_->copy(buffer_, other.buffer_);
                ops_ = other.ops_;
                type_id_ = other.type_id_;
            }
        }
        return *this;
    }

    fixed_any& operator=(fixed_any&& other) {
        if (this != &other) {
            reset();
            if (other.has_value()) {
                other.ops_->move(buffer_, other.buffer_);
                ops_ = other.ops_;
                type_id_ = other.type_id_;
                other.reset();
            }
        }
        return *this;
    }

    template<class T>
    fixed_any& operator=(const T& value) {
        reset();
        construct<T>(value);
        return *this;
    }

    bool has_value() const { return ops_ != nullptr; }

    void reset() {
        if (ops_) {
            ops_->destroy(buffer_);
            ops_ = nullptr;
            type_id_ = nullptr;
        }
    }

    type_id_t type() const { return type_id_; }

private:
    struct ops_table {
        void (*destroy)(void* buf);
        void (*copy)(void* dst, const void* src);
        void (*move)(void* dst, void* src);
    };

    template<class T>
    struct typed_ops {
        static void destroy(void* buf) {
            static_cast<T*>(buf)->~T();
        }
        static void copy(void* dst, const void* src) {
            ::new(dst) T(*static_cast<const T*>(src));
        }
        static void move_fn(void* dst, void* src) {
            ::new(dst) T(prototype::move(*static_cast<T*>(src)));
            static_cast<T*>(src)->~T();
        }
        static const ops_table table;
    };

    template<class T>
    void construct(const T& value) {
        PROTOTYPE_ASSERT(sizeof(T) <= BufferSize);
        ::new(static_cast<void*>(buffer_)) T(value);
        ops_ = &typed_ops<T>::table;
        type_id_ = detail::get_type_id<T>();
    }

    PROTOTYPE_ALIGNAS(16) char buffer_[BufferSize];
    const ops_table* ops_;
    type_id_t type_id_;

    template<class T, size_t S>
    friend T* any_cast(fixed_any<S>* a);

    template<class T, size_t S>
    friend const T* any_cast(const fixed_any<S>* a);
};

template<size_t BufferSize>
template<class T>
const typename fixed_any<BufferSize>::ops_table
fixed_any<BufferSize>::typed_ops<T>::table = {
    &typed_ops<T>::destroy,
    &typed_ops<T>::copy,
    &typed_ops<T>::move_fn
};

///////////////////////////////////////////////////////////////////////////////
// any_cast
///////////////////////////////////////////////////////////////////////////////
template<class T, size_t BufferSize>
T* any_cast(fixed_any<BufferSize>* a) {
    if (!a || !a->has_value()) return nullptr;
    if (a->type_id_ != detail::get_type_id<T>()) return nullptr;
    return static_cast<T*>(static_cast<void*>(a->buffer_));
}

template<class T, size_t BufferSize>
const T* any_cast(const fixed_any<BufferSize>* a) {
    if (!a || !a->has_value()) return nullptr;
    if (a->type_id_ != detail::get_type_id<T>()) return nullptr;
    return static_cast<const T*>(static_cast<const void*>(a->buffer_));
}

} // namespace prototype

#endif // PROTOTYPE_CORE_ANY_HPP
