# Comparators

## Overview

Comparator functors provide standard comparison operations as callable objects. They enable algorithms to be parameterized on comparison behavior without function pointer overhead.

## Available Comparators

```cpp
namespace prototype {

template <typename T = void>
struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template <typename T = void>
struct greater {
    bool operator()(const T& a, const T& b) const { return a > b; }
};

template <typename T = void>
struct equal_to {
    bool operator()(const T& a, const T& b) const { return a == b; }
};

template <typename T = void>
struct not_equal_to {
    bool operator()(const T& a, const T& b) const { return a != b; }
};

template <typename T = void>
struct less_equal {
    bool operator()(const T& a, const T& b) const { return a <= b; }
};

template <typename T = void>
struct greater_equal {
    bool operator()(const T& a, const T& b) const { return a >= b; }
};

} // namespace prototype
```

## Example

```cpp
#include <prototype/functors/comparators.hpp>
#include <prototype/containers/flat_map.hpp>

// flat_map sorted in descending key order
prototype::flat_map<int, const char*, prototype::greater<int>> desc_map;
desc_map.insert({3, "three"});
desc_map.insert({1, "one"});
desc_map.insert({2, "two"});
// Iteration order: 3, 2, 1
```

## Transparent Comparators (C++14+)

The `void` specialization enables heterogeneous lookup:

```cpp
template <>
struct less<void> {
    template <typename T, typename U>
    bool operator()(const T& a, const U& b) const { return a < b; }
};
```

## Performance

All comparators are empty types (size = 1 byte, EBO reduces to 0). The `operator()` is always inlined by the compiler at any optimization level.
