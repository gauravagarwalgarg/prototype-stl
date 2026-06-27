# stack_allocator

## Overview

`prototype::stack_allocator<T>` allocates memory in LIFO (stack) order. Deallocations must happen in reverse order of allocations. Provides O(1) alloc and O(1) dealloc with the fastest possible implementation (single pointer bump).

## When to Use

- Nested scopes with deterministic lifetime
- Scratch buffers for recursive algorithms
- Temporary workspace that is freed in reverse order
- Function-local allocations where order is guaranteed

## API Reference

```cpp
namespace prototype {

template <typename T>
class stack_allocator {
public:
    using value_type = T;

    template <size_t N>
    explicit stack_allocator(memory_arena<N>& arena);

    T* allocate(size_t n);
    void deallocate(T* p, size_t n);  // Must be most recent allocation

    // Marker-based API
    using marker = size_t;
    marker get_marker() const;
    void free_to_marker(marker m);
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/allocators/stack_allocator.hpp>
#include <prototype/allocators/memory_arena.hpp>

prototype::memory_arena<4096> scratch;
prototype::stack_allocator<float> alloc(scratch);

void recursive_fft(float* data, size_t n) {
    auto marker = alloc.get_marker();

    // Allocate temporary workspace
    float* temp = alloc.allocate(n);

    // ... FFT computation using temp ...

    // Free back to marker (regardless of how many allocations happened)
    alloc.free_to_marker(marker);
}
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `allocate(n)` | O(1) | Bump pointer forward |
| `deallocate(p, n)` | O(1) | Bump pointer backward |
| `get_marker` | O(1) | Returns current offset |
| `free_to_marker` | O(1) | Resets to saved offset |

## Safety Notes

!!! warning "LIFO constraint"
    `deallocate()` must free the most recently allocated block. Out-of-order deallocation is undefined behavior. Use the marker API for safer batch-free.

!!! tip "Marker pattern"
    Use `get_marker()` / `free_to_marker()` to batch-free multiple allocations at once without worrying about individual deallocation order.

## Design Notes

- Fastest possible allocator (just pointer arithmetic)
- No free-list, no bookkeeping per allocation
- `free_to_marker` is the recommended deallocation pattern
- Ideal for scratch space in algorithms (sort, FFT, matrix operations)
- Can be combined with recursion by saving/restoring markers at each call
