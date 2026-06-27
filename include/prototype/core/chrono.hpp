///////////////////////////////////////////////////////////////////////////////
// include/prototype/core/chrono.hpp
//
// PrototypeSTL - Lightweight time utilities
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_CORE_CHRONO_HPP
#define PROTOTYPE_CORE_CHRONO_HPP

#include "../config.hpp"
#include "ratio.hpp"

#if defined(PROTOTYPE_PLATFORM_LINUX) || defined(PROTOTYPE_PLATFORM_MACOS)
#include <time.h>
#elif defined(PROTOTYPE_PLATFORM_WINDOWS)
// Forward declare Windows types to avoid including windows.h
extern "C" {
    int __stdcall QueryPerformanceCounter(long long*);
    int __stdcall QueryPerformanceFrequency(long long*);
}
#endif

namespace prototype {
namespace chrono {

///////////////////////////////////////////////////////////////////////////////
// duration<Rep, Period>
///////////////////////////////////////////////////////////////////////////////
template<class Rep, class Period = ratio<1, 1>>
class duration {
public:
    typedef Rep    rep;
    typedef Period period;

    constexpr duration() : count_(0) {}
    explicit constexpr duration(const Rep& r) : count_(r) {}

    template<class Rep2, class Period2>
    duration(const duration<Rep2, Period2>& other)
        : count_(static_cast<Rep>(other.count() *
                 static_cast<Rep>(Period2::num) * static_cast<Rep>(period::den) /
                 (static_cast<Rep>(Period2::den) * static_cast<Rep>(period::num)))) {}

    constexpr Rep count() const { return count_; }

    duration operator+(const duration& other) const {
        return duration(count_ + other.count_);
    }

    duration operator-(const duration& other) const {
        return duration(count_ - other.count_);
    }

    duration operator*(const Rep& rhs) const {
        return duration(count_ * rhs);
    }

    duration operator/(const Rep& rhs) const {
        return duration(count_ / rhs);
    }

    duration& operator+=(const duration& other) {
        count_ += other.count_;
        return *this;
    }

    duration& operator-=(const duration& other) {
        count_ -= other.count_;
        return *this;
    }

    bool operator==(const duration& other) const { return count_ == other.count_; }
    bool operator!=(const duration& other) const { return count_ != other.count_; }
    bool operator<(const duration& other) const { return count_ < other.count_; }
    bool operator<=(const duration& other) const { return count_ <= other.count_; }
    bool operator>(const duration& other) const { return count_ > other.count_; }
    bool operator>=(const duration& other) const { return count_ >= other.count_; }

    static constexpr duration zero() { return duration(0); }

private:
    Rep count_;
};

// Common duration types
typedef duration<long long, nano>       nanoseconds;
typedef duration<long long, micro>      microseconds;
typedef duration<long long, milli>      milliseconds;
typedef duration<long long>             seconds;
typedef duration<long long, ratio<60>>  minutes;
typedef duration<long long, ratio<3600>> hours;

///////////////////////////////////////////////////////////////////////////////
// duration_cast
///////////////////////////////////////////////////////////////////////////////
template<class ToDuration, class Rep, class Period>
ToDuration duration_cast(const duration<Rep, Period>& d) {
    typedef typename ToDuration::rep to_rep;
    typedef typename ToDuration::period to_period;
    return ToDuration(static_cast<to_rep>(
        d.count() *
        static_cast<to_rep>(Period::num) * static_cast<to_rep>(to_period::den) /
        (static_cast<to_rep>(Period::den) * static_cast<to_rep>(to_period::num))
    ));
}

///////////////////////////////////////////////////////////////////////////////
// time_point<Clock, Duration>
///////////////////////////////////////////////////////////////////////////////
template<class Clock, class Duration = typename Clock::duration>
class time_point {
public:
    typedef Clock    clock;
    typedef Duration duration_type;

    time_point() : d_(Duration::zero()) {}
    explicit time_point(const Duration& d) : d_(d) {}

    Duration time_since_epoch() const { return d_; }

    time_point operator+(const Duration& d) const {
        return time_point(d_ + d);
    }

    time_point operator-(const Duration& d) const {
        return time_point(d_ - d);
    }

    Duration operator-(const time_point& other) const {
        return d_ - other.d_;
    }

    bool operator==(const time_point& other) const { return d_ == other.d_; }
    bool operator!=(const time_point& other) const { return d_ != other.d_; }
    bool operator<(const time_point& other) const { return d_ < other.d_; }
    bool operator<=(const time_point& other) const { return d_ <= other.d_; }
    bool operator>(const time_point& other) const { return d_ > other.d_; }
    bool operator>=(const time_point& other) const { return d_ >= other.d_; }

private:
    Duration d_;
};

///////////////////////////////////////////////////////////////////////////////
// steady_clock
///////////////////////////////////////////////////////////////////////////////
struct steady_clock {
    typedef nanoseconds                       duration;
    typedef duration::rep                     rep;
    typedef duration::period                  period;
    typedef time_point<steady_clock>          time_point_type;
    static const bool is_steady = true;

    static time_point_type now() {
#if defined(PROTOTYPE_PLATFORM_LINUX) || defined(PROTOTYPE_PLATFORM_MACOS)
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        long long ns = static_cast<long long>(ts.tv_sec) * 1000000000LL + ts.tv_nsec;
        return time_point_type(nanoseconds(ns));
#elif defined(PROTOTYPE_PLATFORM_WINDOWS)
        long long freq, cnt;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&cnt);
        long long ns = cnt * 1000000000LL / freq;
        return time_point_type(nanoseconds(ns));
#else
        return time_point_type(nanoseconds(0));
#endif
    }
};

} // namespace chrono
} // namespace prototype

#endif // PROTOTYPE_CORE_CHRONO_HPP
