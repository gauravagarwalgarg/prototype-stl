# arena_allocator

## Overview

`prototype::arena_allocator<T>` is a stateful allocator that satisfies allocation requests from a `memory_arena`. It provides O(1) allocation and O(1) bulk deallocation (via arena reset).

## When to Use

- Request/frame/scope-scoped allocations that are freed together
- Game frame allocations
- HTTP request processing
- Compiler phases (parse, analyze, emit reset between phases)

## API Reference

```cpp
namespace prototype {

template <typename T>
class arena_allocator {
public:
    using value_type = T;

    explicit arena_allocator(memory_arena_base& arena);
    
    template <typename U>
    arena_allocator(const arena_allocator<U>& other);  // rebind

    T* allocate(size_t n);
    void deallocate(T* p, size_t n);  // no-op (arena handles bulk free)

    // Comparison
    bool operator==(const arena_allocator& other) const;
    bool operator!=(const arena_allocator& other) const;
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/allocators/memory_arena.hpp>
#include <prototype/allocators/arena_allocator.hpp>
#include <prototype/containers/vector.hpp>
#include <prototype/containers/string.hpp>

prototype::memory_arena<8192> request_arena;

void handle_request() {
    prototype::arena_allocator<char> str_alloc(request_arena);
    prototype::arena_allocator<int> int_alloc(request_arena);

    prototype::vector<int, prototype::arena_allocator<int>> ids(int_alloc);
    ids.push_back(1);
    ids.push_back(2);
    ids.push_back(3);

    // All memory freed in O(1) when we reset
    request_arena.reset();
}
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `allocate(n)` | O(1) | Bump pointer in arena |
| `deallocate(p, n)` | O(1) | No-op |
| Arena `reset()` | O(1) | Frees all allocations |

## Safety Notes

!!! warning "Deallocate is a no-op"
    Individual `deallocate()` calls do nothing. Memory is only reclaimed when the arena is `reset()`. This means the arena's memory usage is monotonically increasing until reset.

!!! tip "Scope lifetime"
    Ensure the arena outlives all allocators and containers that reference it.

## Design Notes

- Stateful: stores reference to the arena
- Allocators from the same arena compare equal (can swap/move containers)
- `deallocate` is intentionally empty this is the key to O(1) performance
- Rebinding (for `list` node allocation etc.) creates a new allocator referencing the same arena
