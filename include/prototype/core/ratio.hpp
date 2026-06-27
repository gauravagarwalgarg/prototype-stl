///////////////////////////////////////////////////////////////////////////////
// include/prototype/core/ratio.hpp
//
// PrototypeSTL - Compile-time rational arithmetic
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_CORE_RATIO_HPP
#define PROTOTYPE_CORE_RATIO_HPP

#include "../config.hpp"

namespace prototype {

namespace detail {
    // Compile-time absolute value
    template<long long N>
    struct ct_abs {
        static const long long value = (N < 0) ? -N : N;
    };

    // Compile-time sign
    template<long long N>
    struct ct_sign {
        static const long long value = (N < 0) ? -1 : (N > 0) ? 1 : 0;
    };

    // Compile-time GCD (Euclidean algorithm)
    template<long long A, long long B>
    struct ct_gcd {
        static const long long value = ct_gcd<B, A % B>::value;
    };

    template<long long A>
    struct ct_gcd<A, 0> {
        static const long long value = A;
    };
} // namespace detail

///////////////////////////////////////////////////////////////////////////////
// ratio<Num, Den>
///////////////////////////////////////////////////////////////////////////////
template<long long Num, long long Den = 1>
struct ratio {
    static const long long num = detail::ct_sign<Den>::value * Num /
                                  detail::ct_gcd<detail::ct_abs<Num>::value,
                                                 detail::ct_abs<Den>::value>::value;
    static const long long den = detail::ct_abs<Den>::value /
                                  detail::ct_gcd<detail::ct_abs<Num>::value,
                                                 detail::ct_abs<Den>::value>::value;
    typedef ratio<num, den> type;
};

///////////////////////////////////////////////////////////////////////////////
// Arithmetic operations
///////////////////////////////////////////////////////////////////////////////
template<class R1, class R2>
struct ratio_add {
    typedef ratio<R1::num * R2::den + R2::num * R1::den, R1::den * R2::den> type;
};

template<class R1, class R2>
struct ratio_subtract {
    typedef ratio<R1::num * R2::den - R2::num * R1::den, R1::den * R2::den> type;
};

template<class R1, class R2>
struct ratio_multiply {
    typedef ratio<R1::num * R2::num, R1::den * R2::den> type;
};

template<class R1, class R2>
struct ratio_divide {
    typedef ratio<R1::num * R2::den, R1::den * R2::num> type;
};

///////////////////////////////////////////////////////////////////////////////
// Comparison
///////////////////////////////////////////////////////////////////////////////
template<class R1, class R2>
struct ratio_equal {
    static const bool value = (R1::num == R2::num && R1::den == R2::den);
};

template<class R1, class R2>
struct ratio_less {
    static const bool value = (R1::num * R2::den < R2::num * R1::den);
};

///////////////////////////////////////////////////////////////////////////////
// Standard ratios
///////////////////////////////////////////////////////////////////////////////
typedef ratio<1, 1000000000LL> nano;
typedef ratio<1, 1000000LL>    micro;
typedef ratio<1, 1000LL>       milli;
typedef ratio<1, 1LL>          unit;
typedef ratio<1000LL, 1>       kilo;
typedef ratio<1000000LL, 1>    mega;
typedef ratio<1000000000LL, 1> giga;

} // namespace prototype

#endif // PROTOTYPE_CORE_RATIO_HPP
