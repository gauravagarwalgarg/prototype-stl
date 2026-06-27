# memory_arena

## Overview

`prototype::memory_arena<Size>` is a fixed-size block of raw memory that serves as the backing storage for allocators. It can be stack-allocated, statically-allocated, or heap-backed.

## When to Use

- You need a fixed pool of memory for a set of related allocations
- Frame/request/scope-based allocation patterns
- Embedded systems with no heap

## API Reference

```cpp
namespace prototype {

template <size_t Size, size_t Alignment = alignof(max_align_t)>
class memory_arena {
public:
    memory_arena();

    // Allocate raw bytes from the arena
    void* allocate(size_t bytes, size_t alignment = Alignment);

    // Reset the arena (invalidates all allocations)
    void reset();

    // Query
    size_t used() const;
    size_t remaining() const;
    static constexpr size_t capacity() { return Size; }
    bool contains(const void* ptr) const;
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/allocators/memory_arena.hpp>

// Stack-allocated arena
prototype::memory_arena<4096> frame_arena;

void render_frame() {
    // All allocations this frame come from the arena
    void* p1 = frame_arena.allocate(256);
    void* p2 = frame_arena.allocate(512);
    
    // ... render ...
    
    // Single O(1) reset frees everything
    frame_arena.reset();
}
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `allocate` | O(1) | Bump pointer |
| `reset` | O(1) | Resets pointer to start |
| `used` | O(1) | Current offset |
| `remaining` | O(1) | Size - offset |

## Safety Notes

!!! warning "No individual free"
    Arena memory cannot be freed individually. Only `reset()` frees all allocations at once. This is by design it's what makes arenas fast.

!!! info "Overflow"
    If an allocation request exceeds remaining capacity, the arena returns `nullptr` in release mode and asserts in debug mode.

## Design Notes

- Internal: `alignas(Alignment) char buffer_[Size]` + offset counter
- `sizeof(memory_arena<4096>)` = 4096 + sizeof(size_t) ≈ 4104 bytes
- Thread-safety: Not thread-safe. Use one arena per thread.
- Alignment: Allocations are aligned to the requested alignment (default: max natural alignment)
