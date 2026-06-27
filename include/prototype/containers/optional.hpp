///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/optional.hpp
//
// PrototypeSTL - optional<T> (nullable value wrapper)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_OPTIONAL_HPP
#define PROTOTYPE_OPTIONAL_HPP

#include "../config.hpp"
#include "../type_traits/type_traits.hpp"
#include "../utility/utility.hpp"

namespace prototype {

struct nullopt_t { explicit nullopt_t() {} };
static const nullopt_t nullopt = nullopt_t();

template<class T>
class optional {
public:
    optional() : engaged_(false) {}
    optional(nullopt_t) : engaged_(false) {}

    optional(const T& value) : engaged_(false) {
        construct(value);
    }

    optional(T&& value) : engaged_(false) {
        construct(prototype::move(value));
    }

    optional(const optional& other) : engaged_(false) {
        if (other.engaged_) construct(*other.ptr());
    }

    optional(optional&& other) : engaged_(false) {
        if (other.engaged_) {
            construct(prototype::move(*other.ptr()));
            other.reset();
        }
    }

    ~optional() { reset(); }

    optional& operator=(nullopt_t) { reset(); return *this; }

    optional& operator=(const optional& other) {
        if (this != &other) {
            reset();
            if (other.engaged_) construct(*other.ptr());
        }
        return *this;
    }

    optional& operator=(optional&& other) {
        if (this != &other) {
            reset();
            if (other.engaged_) {
                construct(prototype::move(*other.ptr()));
                other.reset();
            }
        }
        return *this;
    }

    optional& operator=(const T& value) {
        reset();
        construct(value);
        return *this;
    }

    // Observers
    bool has_value() const { return engaged_; }
    explicit operator bool() const { return engaged_; }

    T& value() { PROTOTYPE_ASSERT(engaged_); return *ptr(); }
    const T& value() const { PROTOTYPE_ASSERT(engaged_); return *ptr(); }

    T& operator*() { return *ptr(); }
    const T& operator*() const { return *ptr(); }
    T* operator->() { return ptr(); }
    const T* operator->() const { return ptr(); }

    T value_or(const T& default_value) const {
        return engaged_ ? *ptr() : default_value;
    }

    // Modifiers
    void reset() {
        if (engaged_) {
            ptr()->~T();
            engaged_ = false;
        }
    }

    template<class... Args>
    void emplace(Args&&... args) {
        reset();
        ::new(static_cast<void*>(storage())) T(prototype::forward<Args>(args)...);
        engaged_ = true;
    }

private:
    void construct(const T& v) {
        ::new(static_cast<void*>(storage())) T(v);
        engaged_ = true;
    }

    void construct(T&& v) {
        ::new(static_cast<void*>(storage())) T(prototype::move(v));
        engaged_ = true;
    }

    void* storage() { return &storage_; }
    const void* storage() const { return &storage_; }
    T* ptr() { return static_cast<T*>(storage()); }
    const T* ptr() const { return static_cast<const T*>(storage()); }

    typename aligned_storage<sizeof(T), alignof(T)>::type storage_;
    bool engaged_;
};

} // namespace prototype

#endif // PROTOTYPE_OPTIONAL_HPP
