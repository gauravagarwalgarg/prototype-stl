# Allocators Overview

## Philosophy

Memory allocation is the single biggest source of performance unpredictability in C++ programs. PrototypeSTL provides a layered allocator architecture (inspired by BDE/Bloomberg) that gives you complete control over when, where, and how memory is allocated.

## Three-Layer Architecture

```
┌─────────────────────────────────────────┐
│         Container (vector, list, ...)    │  Uses allocator interface
└───────────────────┬─────────────────────┘
                    │ allocate(n) / deallocate(p, n)
┌───────────────────▼─────────────────────┐
│         Allocator (arena, pool, stack)   │  Manages allocation strategy
└───────────────────┬─────────────────────┘
                    │ raw byte requests
┌───────────────────▼─────────────────────┐
│         Memory Resource (arena buffer)   │  Owns the actual memory
└─────────────────────────────────────────┘
```

## Available Allocators

| Allocator | Strategy | Use Case |
|-----------|----------|----------|
| `default_allocator<T>` | `malloc`/`free` | General purpose |
| `arena_allocator<T>` | Bump pointer, bulk free | Request-scoped, frame-scoped |
| `pool_allocator<T>` | Free-list of fixed blocks | Many same-size objects (list nodes) |
| `stack_allocator<T>` | LIFO allocation | Nested scopes, scratch buffers |

## Choosing an Allocator

```
Is allocation lifetime scoped?
├── Yes → Are all allocations freed at once?
│         ├── Yes → arena_allocator (fastest possible)
│         └── No  → Are objects same size?
│                   ├── Yes → pool_allocator (O(1) alloc + free)
│                   └── No  → stack_allocator (LIFO free order)
└── No  → default_allocator (general purpose)
```

## Basic Usage

```cpp
#include <prototype/allocators/memory_arena.hpp>
#include <prototype/allocators/arena_allocator.hpp>
#include <prototype/containers/vector.hpp>

// 1. Create the memory resource
prototype::memory_arena<4096> arena;  // 4KB on the stack

// 2. Create the allocator bound to the resource
prototype::arena_allocator<int> alloc(arena);

// 3. Pass allocator to container
prototype::vector<int, prototype::arena_allocator<int>> v(alloc);
v.push_back(42);  // Allocated from the arena, not the heap
```

## Performance Comparison

| Allocator | alloc (ns) | dealloc (ns) | Fragmentation |
|-----------|-----------|-------------|---------------|
| `malloc/free` | 80-100 | 50-70 | Cumulative |
| `arena_allocator` | 2-5 | 0 (bulk) | None |
| `pool_allocator` | 5-10 | 3-5 | None |
| `stack_allocator` | 2-3 | 1-2 | None (LIFO) |

## Safety Notes

!!! warning "Lifetime management"
    Allocator resources must outlive all containers using them. If the arena is destroyed while a vector still references it, you get dangling pointers.

!!! tip "Debug mode"
    In debug builds, allocators track usage and detect:
    - Double-free
    - Use-after-free
    - Arena overflow
    - Pool exhaustion
