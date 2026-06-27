# array

## Overview

`prototype::array<T, N>` is a fixed-size container wrapping a C-style array with bounds checking in debug mode. Equivalent to `std::array` but works without exceptions.

## When to Use

- Size is known at compile time and never changes
- You want array semantics with STL-compatible iteration
- Direct replacement for C arrays with added safety

## API Reference

```cpp
namespace prototype {

template <typename T, size_t N>
class array {
public:
    // Element access
    T& operator[](size_t pos);
    const T& operator[](size_t pos) const;
    T& front();
    T& back();
    T* data();

    // Capacity
    static constexpr size_t size() { return N; }
    static constexpr bool empty() { return N == 0; }

    // Iterators
    T* begin();
    T* end();
    const T* cbegin() const;
    const T* cend() const;

    // Operations
    void fill(const T& value);
    void swap(array& other);
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/array.hpp>

prototype::array<int, 4> coords = {10, 20, 30, 40};

// Iterate
for (auto it = coords.begin(); it != coords.end(); ++it) {
    process(*it);
}

// Fill
prototype::array<float, 8> buffer;
buffer.fill(0.0f);
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `operator[]` | O(1) | Bounds-checked in debug |
| `fill` | O(n) | |
| `size` | O(1) | Compile-time constant |
| `data` | O(1) | Returns raw pointer |

## Safety Notes

!!! info
    `prototype::array` is an aggregate type it supports aggregate initialization and structured bindings (C++17).

## Design Notes

- Zero overhead over raw C arrays
- `sizeof(array<int, 4>)` == `sizeof(int[4])` == 16 bytes
- No allocator parameter (size is always fixed and inline)
- Satisfies ContiguousContainer requirements
