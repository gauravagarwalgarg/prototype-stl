///////////////////////////////////////////////////////////////////////////////
// include/prototype/core/numeric_limits.hpp
//
// PrototypeSTL - Compile-time numeric properties
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_CORE_NUMERIC_LIMITS_HPP
#define PROTOTYPE_CORE_NUMERIC_LIMITS_HPP

#include "../config.hpp"

namespace prototype {

template<class T>
struct numeric_limits {
    static const bool is_specialized = false;
    static const bool is_signed = false;
    static const bool is_integer = false;
    static const int digits = 0;
    static constexpr T min() { return T(); }
    static constexpr T max() { return T(); }
    static constexpr T lowest() { return T(); }
    static constexpr T epsilon() { return T(); }
};

// signed char
template<>
struct numeric_limits<signed char> {
    static const bool is_specialized = true;
    static const bool is_signed = true;
    static const bool is_integer = true;
    static const int digits = 7;
    static constexpr signed char min() { return -128; }
    static constexpr signed char max() { return 127; }
    static constexpr signed char lowest() { return -128; }
    static constexpr signed char epsilon() { return 0; }
};

// unsigned char
template<>
struct numeric_limits<unsigned char> {
    static const bool is_specialized = true;
    static const bool is_signed = false;
    static const bool is_integer = true;
    static const int digits = 8;
    static constexpr unsigned char min() { return 0; }
    static constexpr unsigned char max() { return 255; }
    static constexpr unsigned char lowest() { return 0; }
    static constexpr unsigned char epsilon() { return 0; }
};

// char
template<>
struct numeric_limits<char> {
    static const bool is_specialized = true;
    static const bool is_signed = (static_cast<char>(-1) < 0);
    static const bool is_integer = true;
    static const int digits = is_signed ? 7 : 8;
    static constexpr char min() { return is_signed ? -128 : 0; }
    static constexpr char max() { return is_signed ? 127 : static_cast<char>(255); }
    static constexpr char lowest() { return min(); }
    static constexpr char epsilon() { return 0; }
};

// short
template<>
struct numeric_limits<short> {
    static const bool is_specialized = true;
    static const bool is_signed = true;
    static const bool is_integer = true;
    static const int digits = 15;
    static constexpr short min() { return -32768; }
    static constexpr short max() { return 32767; }
    static constexpr short lowest() { return -32768; }
    static constexpr short epsilon() { return 0; }
};

// unsigned short
template<>
struct numeric_limits<unsigned short> {
    static const bool is_specialized = true;
    static const bool is_signed = false;
    static const bool is_integer = true;
    static const int digits = 16;
    static constexpr unsigned short min() { return 0; }
    static constexpr unsigned short max() { return 65535; }
    static constexpr unsigned short lowest() { return 0; }
    static constexpr unsigned short epsilon() { return 0; }
};

// int
template<>
struct numeric_limits<int> {
    static const bool is_specialized = true;
    static const bool is_signed = true;
    static const bool is_integer = true;
    static const int digits = 31;
    static constexpr int min() { return -2147483647 - 1; }
    static constexpr int max() { return 2147483647; }
    static constexpr int lowest() { return -2147483647 - 1; }
    static constexpr int epsilon() { return 0; }
};

// unsigned int
template<>
struct numeric_limits<unsigned int> {
    static const bool is_specialized = true;
    static const bool is_signed = false;
    static const bool is_integer = true;
    static const int digits = 32;
    static constexpr unsigned int min() { return 0; }
    static constexpr unsigned int max() { return 4294967295U; }
    static constexpr unsigned int lowest() { return 0; }
    static constexpr unsigned int epsilon() { return 0; }
};

// long
template<>
struct numeric_limits<long> {
    static const bool is_specialized = true;
    static const bool is_signed = true;
    static const bool is_integer = true;
    static const int digits = sizeof(long) * 8 - 1;
    static constexpr long min() { return -__LONG_MAX__ - 1L; }
    static constexpr long max() { return __LONG_MAX__; }
    static constexpr long lowest() { return -__LONG_MAX__ - 1L; }
    static constexpr long epsilon() { return 0; }
};

// unsigned long
template<>
struct numeric_limits<unsigned long> {
    static const bool is_specialized = true;
    static const bool is_signed = false;
    static const bool is_integer = true;
    static const int digits = sizeof(unsigned long) * 8;
    static constexpr unsigned long min() { return 0; }
    static constexpr unsigned long max() { return static_cast<unsigned long>(__LONG_MAX__) * 2UL + 1UL; }
    static constexpr unsigned long lowest() { return 0; }
    static constexpr unsigned long epsilon() { return 0; }
};

// long long
template<>
struct numeric_limits<long long> {
    static const bool is_specialized = true;
    static const bool is_signed = true;
    static const bool is_integer = true;
    static const int digits = 63;
    static constexpr long long min() { return -__LONG_LONG_MAX__ - 1LL; }
    static constexpr long long max() { return __LONG_LONG_MAX__; }
    static constexpr long long lowest() { return -__LONG_LONG_MAX__ - 1LL; }
    static constexpr long long epsilon() { return 0; }
};

// unsigned long long
template<>
struct numeric_limits<unsigned long long> {
    static const bool is_specialized = true;
    static const bool is_signed = false;
    static const bool is_integer = true;
    static const int digits = 64;
    static constexpr unsigned long long min() { return 0; }
    static constexpr unsigned long long max() { return static_cast<unsigned long long>(__LONG_LONG_MAX__) * 2ULL + 1ULL; }
    static constexpr unsigned long long lowest() { return 0; }
    static constexpr unsigned long long epsilon() { return 0; }
};

// float
template<>
struct numeric_limits<float> {
    static const bool is_specialized = true;
    static const bool is_signed = true;
    static const bool is_integer = false;
    static const int digits = 24;
    static constexpr float min() { return 1.17549435e-38F; }
    static constexpr float max() { return 3.40282347e+38F; }
    static constexpr float lowest() { return -3.40282347e+38F; }
    static constexpr float epsilon() { return 1.19209290e-7F; }
};

// double
template<>
struct numeric_limits<double> {
    static const bool is_specialized = true;
    static const bool is_signed = true;
    static const bool is_integer = false;
    static const int digits = 53;
    static constexpr double min() { return 2.2250738585072014e-308; }
    static constexpr double max() { return 1.7976931348623157e+308; }
    static constexpr double lowest() { return -1.7976931348623157e+308; }
    static constexpr double epsilon() { return 2.2204460492503131e-16; }
};

// long double
template<>
struct numeric_limits<long double> {
    static const bool is_specialized = true;
    static const bool is_signed = true;
    static const bool is_integer = false;
    static const int digits = 64;
    static constexpr long double min() { return 3.3621031431120935063e-4932L; }
    static constexpr long double max() { return 1.1897314953572317650e+4932L; }
    static constexpr long double lowest() { return -1.1897314953572317650e+4932L; }
    static constexpr long double epsilon() { return 1.0842021724855044340e-19L; }
};

} // namespace prototype

#endif // PROTOTYPE_CORE_NUMERIC_LIMITS_HPP
