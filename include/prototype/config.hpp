///////////////////////////////////////////////////////////////////////////////
// include/prototype/config.hpp
//
// PrototypeSTL - Safety-Critical C++ Standard Template Library
// Configuration, feature detection, macros, and panic system.
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_CONFIG_HPP
#define PROTOTYPE_CONFIG_HPP

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
// Version
///////////////////////////////////////////////////////////////////////////////
#define PROTOTYPE_VERSION_MAJOR 1
#define PROTOTYPE_VERSION_MINOR 0
#define PROTOTYPE_VERSION_PATCH 0

///////////////////////////////////////////////////////////////////////////////
// Compiler detection
///////////////////////////////////////////////////////////////////////////////
#if defined(__GNUC__)
#  define PROTOTYPE_COMPILER_GCC 1
#  define PROTOTYPE_LIKELY(x)   __builtin_expect(!!(x), 1)
#  define PROTOTYPE_UNLIKELY(x) __builtin_expect(!!(x), 0)
#  define PROTOTYPE_ALWAYS_INLINE __attribute__((always_inline)) inline
#  define PROTOTYPE_NOINLINE      __attribute__((noinline))
#elif defined(_MSC_VER)
#  define PROTOTYPE_COMPILER_MSVC 1
#  define PROTOTYPE_LIKELY(x)   (x)
#  define PROTOTYPE_UNLIKELY(x) (x)
#  define PROTOTYPE_ALWAYS_INLINE __forceinline
#  define PROTOTYPE_NOINLINE      __declspec(noinline)
#else
#  define PROTOTYPE_LIKELY(x)   (x)
#  define PROTOTYPE_UNLIKELY(x) (x)
#  define PROTOTYPE_ALWAYS_INLINE inline
#  define PROTOTYPE_NOINLINE
#endif

///////////////////////////////////////////////////////////////////////////////
// Platform detection
///////////////////////////////////////////////////////////////////////////////
#if defined(__linux__)
#  define PROTOTYPE_PLATFORM_LINUX 1
#elif defined(_WIN32)
#  define PROTOTYPE_PLATFORM_WINDOWS 1
#elif defined(__APPLE__)
#  define PROTOTYPE_PLATFORM_MACOS 1
#endif

///////////////////////////////////////////////////////////////////////////////
// C++ standard detection
///////////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 201402L
#  define PROTOTYPE_CPP14 1
#endif
#if __cplusplus >= 201703L
#  define PROTOTYPE_CPP17 1
#endif

///////////////////////////////////////////////////////////////////////////////
// Constexpr support
///////////////////////////////////////////////////////////////////////////////
#if defined(PROTOTYPE_CPP14)
#  define PROTOTYPE_CONSTEXPR14 constexpr
#else
#  define PROTOTYPE_CONSTEXPR14
#endif

///////////////////////////////////////////////////////////////////////////////
// Alignment
///////////////////////////////////////////////////////////////////////////////
#if defined(PROTOTYPE_COMPILER_GCC)
#  define PROTOTYPE_ALIGNAS(N) __attribute__((aligned(N)))
#elif defined(PROTOTYPE_COMPILER_MSVC)
#  define PROTOTYPE_ALIGNAS(N) __declspec(align(N))
#else
#  define PROTOTYPE_ALIGNAS(N) alignas(N)
#endif

#define PROTOTYPE_CACHE_LINE 64

///////////////////////////////////////////////////////////////////////////////
// Assertions and panic
///////////////////////////////////////////////////////////////////////////////
namespace prototype {

typedef void (*panic_handler_t)(const char* msg, const char* file, int line);

inline panic_handler_t& get_panic_handler() {
    static panic_handler_t handler = nullptr;
    return handler;
}

inline void set_panic_handler(panic_handler_t h) {
    get_panic_handler() = h;
}

inline void default_panic(const char* msg, const char* file, int line) {
    fprintf(stderr, "PROTOTYPE PANIC: %s at %s:%d\n", msg, file, line);
    abort();
}

inline void panic(const char* msg, const char* file, int line) {
    panic_handler_t h = get_panic_handler();
    if (h) {
        h(msg, file, line);
    } else {
        default_panic(msg, file, line);
    }
}

} // namespace prototype

#define PROTOTYPE_PANIC(msg) ::prototype::panic(msg, __FILE__, __LINE__)

#define PROTOTYPE_ASSERT(cond) \
    do { if (PROTOTYPE_UNLIKELY(!(cond))) { PROTOTYPE_PANIC(#cond); } } while(0)

#ifdef NDEBUG
#  define PROTOTYPE_DEBUG_ASSERT(cond) ((void)0)
#else
#  define PROTOTYPE_DEBUG_ASSERT(cond) PROTOTYPE_ASSERT(cond)
#endif

///////////////////////////////////////////////////////////////////////////////
// Placement new (no <new> header needed)
///////////////////////////////////////////////////////////////////////////////
#ifndef PROTOTYPE_PLACEMENT_NEW_DEFINED
#define PROTOTYPE_PLACEMENT_NEW_DEFINED
inline void* operator new(size_t, void* p) { return p; }
inline void operator delete(void*, void*) {}
#endif

#endif // PROTOTYPE_CONFIG_HPP
