///////////////////////////////////////////////////////////////////////////////
// include/prototype/type_traits/type_traits.hpp
//
// PrototypeSTL - Complete type_traits library
// Provides compile-time type introspection without RTTI.
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_TYPE_TRAITS_HPP
#define PROTOTYPE_TYPE_TRAITS_HPP

#include "../config.hpp"

namespace prototype {

///////////////////////////////////////////////////////////////////////////////
// integral_constant, true_type, false_type
///////////////////////////////////////////////////////////////////////////////
template<class T, T v>
struct integral_constant {
    static const T value = v;
    typedef T value_type;
    typedef integral_constant type;
    operator value_type() const { return value; }
};

typedef integral_constant<bool, true>  true_type;
typedef integral_constant<bool, false> false_type;

///////////////////////////////////////////////////////////////////////////////
// remove_const, remove_volatile, remove_cv
///////////////////////////////////////////////////////////////////////////////
template<class T> struct remove_const          { typedef T type; };
template<class T> struct remove_const<const T> { typedef T type; };

template<class T> struct remove_volatile             { typedef T type; };
template<class T> struct remove_volatile<volatile T> { typedef T type; };

template<class T> struct remove_cv {
    typedef typename remove_const<typename remove_volatile<T>::type>::type type;
};

///////////////////////////////////////////////////////////////////////////////
// remove_reference, add_lvalue_reference, add_rvalue_reference
///////////////////////////////////////////////////////////////////////////////
template<class T> struct remove_reference      { typedef T type; };
template<class T> struct remove_reference<T&>  { typedef T type; };
template<class T> struct remove_reference<T&&> { typedef T type; };

template<class T> struct add_lvalue_reference      { typedef T& type; };
template<class T> struct add_lvalue_reference<T&>  { typedef T& type; };

template<class T> struct add_rvalue_reference      { typedef T&& type; };
template<class T> struct add_rvalue_reference<T&>  { typedef T& type; };

///////////////////////////////////////////////////////////////////////////////
// remove_pointer
///////////////////////////////////////////////////////////////////////////////
template<class T> struct remove_pointer                    { typedef T type; };
template<class T> struct remove_pointer<T*>               { typedef T type; };
template<class T> struct remove_pointer<T* const>         { typedef T type; };
template<class T> struct remove_pointer<T* volatile>      { typedef T type; };
template<class T> struct remove_pointer<T* const volatile>{ typedef T type; };

///////////////////////////////////////////////////////////////////////////////
// is_same
///////////////////////////////////////////////////////////////////////////////
template<class T, class U> struct is_same       : false_type {};
template<class T>          struct is_same<T, T> : true_type {};

///////////////////////////////////////////////////////////////////////////////
// is_void
///////////////////////////////////////////////////////////////////////////////
namespace detail {
    template<class T> struct is_void_helper : false_type {};
    template<> struct is_void_helper<void>  : true_type {};
}
template<class T> struct is_void : detail::is_void_helper<typename remove_cv<T>::type> {};

///////////////////////////////////////////////////////////////////////////////
// is_integral
///////////////////////////////////////////////////////////////////////////////
namespace detail {
    template<class T> struct is_integral_helper : false_type {};
    template<> struct is_integral_helper<bool>               : true_type {};
    template<> struct is_integral_helper<char>               : true_type {};
    template<> struct is_integral_helper<signed char>        : true_type {};
    template<> struct is_integral_helper<unsigned char>      : true_type {};
    template<> struct is_integral_helper<short>              : true_type {};
    template<> struct is_integral_helper<unsigned short>     : true_type {};
    template<> struct is_integral_helper<int>                : true_type {};
    template<> struct is_integral_helper<unsigned int>       : true_type {};
    template<> struct is_integral_helper<long>               : true_type {};
    template<> struct is_integral_helper<unsigned long>      : true_type {};
    template<> struct is_integral_helper<long long>          : true_type {};
    template<> struct is_integral_helper<unsigned long long> : true_type {};
}
template<class T> struct is_integral : detail::is_integral_helper<typename remove_cv<T>::type> {};

///////////////////////////////////////////////////////////////////////////////
// is_floating_point
///////////////////////////////////////////////////////////////////////////////
namespace detail {
    template<class T> struct is_floating_point_helper : false_type {};
    template<> struct is_floating_point_helper<float>       : true_type {};
    template<> struct is_floating_point_helper<double>      : true_type {};
    template<> struct is_floating_point_helper<long double> : true_type {};
}
template<class T> struct is_floating_point : detail::is_floating_point_helper<typename remove_cv<T>::type> {};

///////////////////////////////////////////////////////////////////////////////
// is_arithmetic
///////////////////////////////////////////////////////////////////////////////
template<class T> struct is_arithmetic
    : integral_constant<bool, is_integral<T>::value || is_floating_point<T>::value> {};

///////////////////////////////////////////////////////////////////////////////
// is_pointer
///////////////////////////////////////////////////////////////////////////////
namespace detail {
    template<class T> struct is_pointer_helper     : false_type {};
    template<class T> struct is_pointer_helper<T*> : true_type {};
}
template<class T> struct is_pointer : detail::is_pointer_helper<typename remove_cv<T>::type> {};

///////////////////////////////////////////////////////////////////////////////
// is_reference
///////////////////////////////////////////////////////////////////////////////
template<class T> struct is_lvalue_reference      : false_type {};
template<class T> struct is_lvalue_reference<T&>  : true_type {};

template<class T> struct is_rvalue_reference      : false_type {};
template<class T> struct is_rvalue_reference<T&&> : true_type {};

template<class T> struct is_reference
    : integral_constant<bool, is_lvalue_reference<T>::value || is_rvalue_reference<T>::value> {};

///////////////////////////////////////////////////////////////////////////////
// is_const, is_volatile
///////////////////////////////////////////////////////////////////////////////
template<class T> struct is_const          : false_type {};
template<class T> struct is_const<const T> : true_type {};

template<class T> struct is_volatile             : false_type {};
template<class T> struct is_volatile<volatile T> : true_type {};

///////////////////////////////////////////////////////////////////////////////
// is_array
///////////////////////////////////////////////////////////////////////////////
template<class T> struct is_array                 : false_type {};
template<class T> struct is_array<T[]>            : true_type {};
template<class T, size_t N> struct is_array<T[N]> : true_type {};

///////////////////////////////////////////////////////////////////////////////
// is_trivially_copyable (conservative: true for arithmetic and pointers)
///////////////////////////////////////////////////////////////////////////////
template<class T> struct is_trivially_copyable
    : integral_constant<bool, is_arithmetic<T>::value || is_pointer<T>::value> {};

///////////////////////////////////////////////////////////////////////////////
// is_trivially_destructible
///////////////////////////////////////////////////////////////////////////////
template<class T> struct is_trivially_destructible
    : integral_constant<bool, is_arithmetic<T>::value || is_pointer<T>::value> {};

///////////////////////////////////////////////////////////////////////////////
// conditional
///////////////////////////////////////////////////////////////////////////////
template<bool B, class T, class F> struct conditional          { typedef T type; };
template<class T, class F>         struct conditional<false, T, F> { typedef F type; };

///////////////////////////////////////////////////////////////////////////////
// enable_if
///////////////////////////////////////////////////////////////////////////////
template<bool B, class T = void> struct enable_if {};
template<class T>                struct enable_if<true, T> { typedef T type; };

///////////////////////////////////////////////////////////////////////////////
// decay (simplified)
///////////////////////////////////////////////////////////////////////////////
template<class T>
struct decay {
    typedef typename remove_reference<T>::type U;
    typedef typename remove_cv<U>::type type;
};

///////////////////////////////////////////////////////////////////////////////
// aligned_storage
///////////////////////////////////////////////////////////////////////////////
template<size_t Len, size_t Align>
struct aligned_storage {
    struct type {
        PROTOTYPE_ALIGNAS(Align) unsigned char data[Len];
    };
};

///////////////////////////////////////////////////////////////////////////////
// declval (declaration only - used in unevaluated contexts)
///////////////////////////////////////////////////////////////////////////////
template<class T>
typename add_rvalue_reference<T>::type declval();

} // namespace prototype

#endif // PROTOTYPE_TYPE_TRAITS_HPP
