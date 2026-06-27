# vector

## Overview

`prototype::vector<T, Allocator>` is a dynamic array with contiguous storage. It provides O(1) amortized push_back, O(1) random access, and full allocator customization.

## When to Use

- You need a dynamically-sized collection with fast random access
- Elements must be stored contiguously (cache-friendly iteration)
- You want to control the allocation strategy

## API Reference

```cpp
namespace prototype {

template <typename T, typename Allocator = default_allocator<T>>
class vector {
public:
    // Construction
    vector();
    explicit vector(const Allocator& alloc);
    explicit vector(size_t count, const T& value = T());
    vector(const vector& other);
    vector(vector&& other);

    // Assignment
    vector& operator=(const vector& other);
    vector& operator=(vector&& other);

    // Element access
    T& operator[](size_t pos);
    const T& operator[](size_t pos) const;
    T& front();
    T& back();
    T* data();

    // Iterators
    iterator begin();
    iterator end();
    const_iterator cbegin() const;
    const_iterator cend() const;

    // Capacity
    bool empty() const;
    size_t size() const;
    size_t capacity() const;
    void reserve(size_t new_cap);
    void shrink_to_fit();

    // Modifiers
    void push_back(const T& value);
    void push_back(T&& value);
    template <typename... Args>
    void emplace_back(Args&&... args);  // C++11
    void pop_back();
    void clear();
    void resize(size_t count);
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    iterator insert(iterator pos, const T& value);
    void swap(vector& other);
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/vector.hpp>
#include <prototype/allocators/arena_allocator.hpp>

// Basic usage
prototype::vector<int> prices;
prices.reserve(1000);
prices.push_back(142);
prices.push_back(89);

// With custom allocator
prototype::memory_arena<8192> arena;
prototype::arena_allocator<double> alloc(arena);
prototype::vector<double, prototype::arena_allocator<double>> ticks(alloc);
ticks.push_back(1.2345);
ticks.push_back(1.2346);

// Iterate
for (size_t i = 0; i < prices.size(); ++i) {
    process(prices[i]);
}
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `push_back` | O(1) amortized | Reallocation on capacity exceeded |
| `pop_back` | O(1) | No deallocation |
| `operator[]` | O(1) | Bounds-checked in debug |
| `insert` | O(n) | Shifts elements |
| `erase` | O(n) | Shifts elements |
| `reserve` | O(n) | Copies existing elements |
| `clear` | O(n) | Destroys elements, keeps capacity |
| `begin/end` | O(1) | Raw pointer iterators |

## Growth Strategy

PrototypeSTL uses 2x growth (same as most `std::vector` implementations). The growth factor balances:

- **Memory efficiency** 1.5x wastes less but reallocates more often
- **Amortized cost** 2x guarantees O(1) amortized push_back
- **Reuse potential** 2x prevents reuse of freed blocks (1.5x allows it)

## Safety Notes

!!! warning "Debug vs Release"
    In debug mode (`PROTOTYPE_DEBUG`), `operator[]` performs bounds checking. In release mode, out-of-bounds access is undefined behavior for maximum performance.

!!! tip "Prefer reserve()"
    If you know the approximate size, call `reserve()` upfront to avoid reallocations during hot paths.

## Design Notes

- No Small Buffer Optimization (SBO) vector always heap-allocates (use `fixed_vector` for stack allocation)
- Move operations are O(1) just pointer swap
- Iterator invalidation rules match `std::vector`
