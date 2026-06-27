# Architecture

## Library Organization

```
include/prototype/
├── config.hpp              ← Compile-time configuration & feature detection
├── prototype.hpp           ← Master include (all components)
├── allocators/             ← Memory management layer
├── containers/             ← Data structures
├── algorithms/             ← Generic algorithms
├── iterators/              ← Iterator types and utilities
├── functors/               ← Function objects and callables
├── concurrency/            ← Lock-free primitives
├── type_traits/            ← Compile-time type inspection
└── utility/                ← Core utilities (move, forward, swap, pair)
```

## Layered Design

PrototypeSTL uses a strict layered architecture where dependencies only flow downward:

```
┌─────────────────────────────────────────┐
│           Containers                     │  ← Top layer: uses everything below
├─────────────────────────────────────────┤
│     Algorithms    │    Concurrency       │  ← Mid layer: uses iterators + allocators
├───────────────────┼─────────────────────┤
│     Iterators     │    Functors          │  ← Core layer: uses type_traits + utility
├─────────────────────────────────────────┤
│     Allocators    │    Type Traits       │  ← Foundation: uses only config
├─────────────────────────────────────────┤
│              config.hpp                   │  ← Base: platform detection
└─────────────────────────────────────────┘
```

## Component Model

Each component follows BDE-inspired principles:

1. **Self-contained** Each header includes only what it needs
2. **Minimal coupling** Components depend on abstractions, not concrete types
3. **Testable in isolation** No global state, no singletons

## Allocator Architecture

The allocator model is the library's most distinctive feature:

```
┌─────────────────┐
│   Container     │  ← Parameterized on Allocator type
│ (vector, list)  │
└────────┬────────┘
         │ allocate() / deallocate()
┌────────▼────────┐
│   Allocator     │  ← arena_allocator, pool_allocator, stack_allocator
│   (Adapter)     │
└────────┬────────┘
         │ raw memory requests
┌────────▼────────┐
│  Memory Arena   │  ← Fixed buffer (stack, static, or heap-backed)
│  (Resource)     │
└─────────────────┘
```

This three-layer design (inspired by BDE's `bslma` package) gives you:

- **Containers** that are allocator-agnostic
- **Allocators** that are resource-agnostic  
- **Resources** that own the actual memory

## Include Path Strategy

The canonical include path is:

```cpp
#include <prototype/containers/vector.hpp>
```

STL-compatible wrappers provide a migration path:

```cpp
#include "containers/vector"  // brings prototype::vector into global scope
```

## Compile-Time Configuration

`config.hpp` detects:

- C++ standard version (11/14/17/20)
- Compiler identity and version
- Platform (Linux/Windows/embedded)
- Feature availability (constexpr depth, alignas, etc.)

All detection results are exposed as `PROTOTYPE_*` macros for conditional compilation throughout the library.

## Error Handling Strategy

| Mode | Behavior |
|------|----------|
| Release (`NDEBUG`) | Undefined behavior on contract violation (zero overhead) |
| Debug (`PROTOTYPE_DEBUG`) | Assertion failure with file/line info |
| Safe mode (`PROTOTYPE_SAFE`) | Returns error codes, never UB |

No exceptions are thrown anywhere in the library.
