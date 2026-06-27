# pool_allocator

## Overview

`prototype::pool_allocator<T>` manages a pool of fixed-size memory blocks. It provides O(1) allocation and O(1) deallocation for objects of uniform size, with zero fragmentation.

## When to Use

- Many objects of the same size (list nodes, tree nodes, event objects)
- Frequent alloc/dealloc cycles
- Eliminating heap fragmentation in long-running systems
- Real-time systems needing deterministic allocation time

## API Reference

```cpp
namespace prototype {

template <typename T>
class pool_allocator {
public:
    using value_type = T;

    explicit pool_allocator(size_t pool_size);                    // Heap-backed
    pool_allocator(void* buffer, size_t buffer_size);             // User-provided buffer
    
    template <size_t N>
    explicit pool_allocator(memory_arena<N>& arena);             // Arena-backed

    T* allocate(size_t n = 1);      // n must be 1
    void deallocate(T* p, size_t n = 1);

    // Query
    size_t available() const;       // Free blocks remaining
    size_t capacity() const;        // Total blocks
    bool full() const;
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/allocators/pool_allocator.hpp>
#include <prototype/containers/list.hpp>

// Pool for 1024 list nodes O(1) per alloc/dealloc
prototype::pool_allocator<int> node_pool(1024);
prototype::list<int, prototype::pool_allocator<int>> events(node_pool);

events.push_back(1);   // O(1) from pool
events.push_back(2);   // O(1) from pool
events.pop_front();    // O(1) return to pool

// Perfect for game entity lists, event queues, etc.
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `allocate(1)` | O(1) | Pop from free list |
| `deallocate(p, 1)` | O(1) | Push to free list |
| `available()` | O(1) | Counter |
| Construction | O(n) | Builds free list |

## Safety Notes

!!! warning "Single-object allocation"
    Pool allocator only supports allocating one object at a time (`n=1`). Attempting to allocate arrays (`n > 1`) is unsupported and triggers a debug assertion.

!!! info "Exhaustion"
    When the pool is empty, `allocate()` returns `nullptr` in release mode and asserts in debug mode. Check `full()` or `available()` before allocation in resource-constrained code.

## Design Notes

- Internal: free-list of blocks embedded in the unallocated memory itself
- Block size: `max(sizeof(T), sizeof(void*))` to store free-list pointer
- Zero memory overhead per block (free pointer is stored in the unused block)
- Thread-safety: Not thread-safe. Use per-thread pools.
- Ideal companion for `list`, `forward_list`, and any node-based container
