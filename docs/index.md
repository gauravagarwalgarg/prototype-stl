# PrototypeSTL

**A high-performance, embedded-friendly C++ standard library alternative.**

---

## What is PrototypeSTL?

PrototypeSTL is a header-only C++ library that provides STL-compatible containers, algorithms, and utilities designed for performance-critical and resource-constrained environments. It synthesizes proven patterns from three industry-leading libraries into a single, cohesive toolkit.

## Design Goals

| Goal | How We Achieve It |
|------|-------------------|
| **Zero-cost abstractions** | No virtual dispatch, no hidden heap allocations |
| **Deterministic memory** | Custom allocators with compile-time capacity bounds |
| **Cache efficiency** | Contiguous storage, flat containers, SBO optimization |
| **Portability** | C++11 baseline, no platform-specific dependencies |
| **Safety** | Bounds checking in debug, static analysis friendly |

## Quick Example

```cpp
#include <prototype/containers/vector.hpp>
#include <prototype/allocators/arena_allocator.hpp>
#include <prototype/algorithms/sort.hpp>

// Create a 4KB memory arena on the stack
prototype::memory_arena<4096> arena;
prototype::arena_allocator<int> alloc(arena);

// Vector allocates from the arena zero heap usage
prototype::vector<int, prototype::arena_allocator<int>> prices(alloc);
prices.push_back(142);
prices.push_back(89);
prices.push_back(237);

// Sort in-place
prototype::sort(prices.begin(), prices.end());
```

## Feature Matrix

| Category | Components |
|----------|-----------|
| Containers | vector, fixed_vector, array, list, deque, string, span, optional, variant, bitset, flat_map, flat_set, ring_buffer, static_map, stack, queue |
| Algorithms | sort, find, copy, fill, transform, partition, merge, rotate, heap, numeric |
| Allocators | memory_arena, arena_allocator, pool_allocator, stack_allocator |
| Iterators | reverse_iterator, move_iterator, iterator_traits, advance/distance |
| Functors | comparators, arithmetic, logical, function, hash |
| Concurrency | atomic, spinlock, spsc_queue |

## Getting Started

See the [Getting Started](getting-started.md) guide for installation and first steps.
