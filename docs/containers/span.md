# span

## Overview

`prototype::span<T>` is a non-owning view over a contiguous sequence of elements. It provides a lightweight, bounds-safe way to pass array-like data without copying. Equivalent to C++20's `std::span` but available from C++11.

## When to Use

- You want to pass a view of contiguous data without ownership transfer
- Function parameters that accept "any contiguous container"
- Replacing raw pointer + size pairs with a safer abstraction

## API Reference

```cpp
namespace prototype {

template <typename T>
class span {
public:
    span();
    span(T* data, size_t size);
    span(T* first, T* last);

    template <size_t N>
    span(T (&arr)[N]);                    // From C array
    span(vector<T>& v);                   // From vector
    span(array<T, N>& a);                 // From array

    // Element access
    T& operator[](size_t idx);
    T& front();
    T& back();
    T* data();

    // Capacity
    size_t size() const;
    bool empty() const;

    // Subviews
    span<T> subspan(size_t offset, size_t count = dynamic_extent);
    span<T> first(size_t count);
    span<T> last(size_t count);

    // Iterators
    T* begin();
    T* end();
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/span.hpp>

void process_data(prototype::span<const float> readings) {
    for (size_t i = 0; i < readings.size(); ++i) {
        transmit(readings[i]);
    }
}

// Works with any contiguous source
prototype::vector<float> vec = {1.0f, 2.0f, 3.0f};
process_data(prototype::span<const float>(vec.data(), vec.size()));

float arr[] = {4.0f, 5.0f, 6.0f};
process_data(prototype::span<const float>(arr));
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Construction | O(1) | Just stores pointer + size |
| `operator[]` | O(1) | Bounds-checked in debug |
| `subspan` | O(1) | Pointer arithmetic |
| `size` | O(1) | |

## Safety Notes

!!! warning "Lifetime"
    `span` does not own its data. The underlying storage must outlive the span. Dangling spans are undefined behavior.

!!! tip "Use const span"
    Prefer `span<const T>` for read-only access to communicate intent and prevent accidental modification.

## Design Notes

- `sizeof(span<T>)` = 16 bytes (pointer + size)
- Zero overhead over raw pointer + size
- No allocations ever
- Trivially copyable
