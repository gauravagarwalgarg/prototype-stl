///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/variant.hpp
//
// PrototypeSTL - Type-safe tagged union (simplified variant)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_VARIANT_HPP
#define PROTOTYPE_VARIANT_HPP

#include "../config.hpp"
#include "../type_traits/type_traits.hpp"
#include "../utility/utility.hpp"

namespace prototype {

namespace detail {

template<class... Ts> struct max_size;
template<class T> struct max_size<T> { static const size_t value = sizeof(T); };
template<class T, class... Rest>
struct max_size<T, Rest...> {
    static const size_t rest_value = max_size<Rest...>::value;
    static const size_t value = sizeof(T) > rest_value ? sizeof(T) : rest_value;
};

template<class... Ts> struct max_align;
template<class T> struct max_align<T> { static const size_t value = alignof(T); };
template<class T, class... Rest>
struct max_align<T, Rest...> {
    static const size_t rest_value = max_align<Rest...>::value;
    static const size_t value = alignof(T) > rest_value ? alignof(T) : rest_value;
};

template<class T, class... Ts> struct type_index;
template<class T, class First, class... Rest>
struct type_index<T, First, Rest...> {
    static const size_t value = is_same<T, First>::value ? 0 : 1 + type_index<T, Rest...>::value;
};
template<class T, class First>
struct type_index<T, First> {
    static const size_t value = is_same<T, First>::value ? 0 : 1;
};

} // namespace detail

static const size_t variant_npos = static_cast<size_t>(-1);

template<class... Ts>
class variant {
    static const size_t storage_size  = detail::max_size<Ts...>::value;
    static const size_t storage_align = detail::max_align<Ts...>::value;

public:
    variant() : index_(variant_npos) {}

    template<class T>
    variant(const T& value) : index_(variant_npos) {
        static const size_t idx = detail::type_index<T, Ts...>::value;
        ::new(static_cast<void*>(&storage_)) T(value);
        index_ = idx;
    }

    ~variant() { destroy(); }

    variant(const variant&) = delete; // simplified: no copy
    variant& operator=(const variant&) = delete;

    template<class T>
    void set(const T& value) {
        destroy();
        static const size_t idx = detail::type_index<T, Ts...>::value;
        ::new(static_cast<void*>(&storage_)) T(value);
        index_ = idx;
    }

    template<class T>
    T& get() {
        static const size_t idx = detail::type_index<T, Ts...>::value;
        PROTOTYPE_ASSERT(index_ == idx);
        return *reinterpret_cast<T*>(&storage_);
    }

    template<class T>
    const T& get() const {
        static const size_t idx = detail::type_index<T, Ts...>::value;
        PROTOTYPE_ASSERT(index_ == idx);
        return *reinterpret_cast<const T*>(&storage_);
    }

    size_t index() const { return index_; }
    bool valueless_by_exception() const { return index_ == variant_npos; }

    template<class T>
    bool holds_alternative() const {
        static const size_t idx = detail::type_index<T, Ts...>::value;
        return index_ == idx;
    }

private:
    void destroy() {
        // Note: proper variant would destroy via vtable/function pointer table.
        // Simplified version relies on trivially destructible types.
        index_ = variant_npos;
    }

    typename aligned_storage<storage_size, storage_align>::type storage_;
    size_t index_;
};

} // namespace prototype

#endif // PROTOTYPE_VARIANT_HPP
