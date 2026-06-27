# Design Philosophy

## The Triple-Repo Synthesis

PrototypeSTL doesn't reinvent the wheel. It synthesizes proven patterns from three battle-tested libraries, each representing a different domain expertise:

### EASTL (Electronic Arts Standard Template Library)

**Domain:** Game engines, real-time rendering, 60fps constraints

**What we take:**

- Custom allocator as first-class citizen (not an afterthought)
- `fixed_vector`, `fixed_string` stack-allocated containers with compile-time capacity
- Aggressive inlining and cache-line awareness
- No hidden allocations in any operation

### ETL (Embedded Template Library)

**Domain:** Bare-metal embedded systems, safety-critical firmware

**What we take:**

- Fixed-capacity containers that never allocate from heap
- Compile-time size bounds with static assertions
- No exceptions, no RTTI, no dynamic_cast
- Deterministic execution time for all operations

### BDE (Bloomberg Development Environment)

**Domain:** Enterprise financial systems, high-throughput trading

**What we take:**

- Layered allocator architecture (resource → allocator → container)
- Component-level design with strict dependency management
- `memory_arena` as a first-class memory resource
- Pool allocators for uniform-size objects

## Core Principles

### 1. You Don't Pay for What You Don't Use

Every feature is opt-in. A `prototype::vector<int>` with the default allocator compiles to nearly identical assembly as a raw `int*` with manual management.

### 2. Allocation is Explicit

The standard `std::vector` hides when and how it allocates. PrototypeSTL makes allocation a visible, controllable decision:

```cpp
// Heap-backed (like std::vector)
prototype::vector<int> v1;

// Stack-backed, fixed capacity
prototype::fixed_vector<int, 64> v2;

// Arena-backed, deterministic lifetime
prototype::memory_arena<4096> arena;
prototype::arena_allocator<int> alloc(arena);
prototype::vector<int, prototype::arena_allocator<int>> v3(alloc);
```

### 3. Debug Builds Catch Bugs, Release Builds Fly

```cpp
// Debug: bounds check, iterator validation, use-after-free detection
// Release: all checks compiled away, zero overhead
v[i];  // Asserts i < size() in debug, raw access in release
```

### 4. Cache Lines Matter

Data layout decisions are driven by hardware reality:

- Flat containers (`flat_map`, `flat_set`) over node-based by default
- Small Buffer Optimization (SBO) in `string` and `function`
- Contiguous storage preferred over pointer-chasing

### 5. Embedded-First, Not Embedded-Only

The library works on a Cortex-M0 with 2KB RAM. It also works on a 128-core server. The same code, the same API just different allocator choices.

## What We Explicitly Reject

| STL Pattern | Our Alternative | Why |
|-------------|----------------|-----|
| Global `new`/`delete` | Explicit allocators | Determinism, fragmentation control |
| Exceptions for errors | Return codes + assertions | Code size, real-time constraints |
| RTTI / `dynamic_cast` | Static polymorphism | Binary size, embedded compatibility |
| `std::allocator` model | BDE-inspired resources | Actually usable in practice |
| Node-based containers as default | Flat containers | Cache performance |

## Naming Conventions

- **Types:** `snake_case` (e.g., `fixed_vector`, `arena_allocator`)
- **Functions:** `snake_case` (e.g., `push_back`, `emplace`)
- **Macros:** `PROTOTYPE_UPPER_CASE`
- **Namespaces:** `prototype`, `prototype::allocators`, etc.
- **Files:** `snake_case.hpp`
