# Why Not STL?

## The Problem with `std::`

The C++ Standard Library is excellent for general-purpose programming. But in performance-critical and resource-constrained domains, it has fundamental limitations that can't be worked around:

## 1. Unpredictable Memory Allocation

```cpp
std::vector<int> v;
v.push_back(1);  // When does this allocate? How much? Where?
```

The standard says nothing about allocation strategy. Most implementations use geometric growth (1.5x or 2x), but you can't control:

- **When** allocation happens (during a latency-critical path?)
- **Where** memory comes from (heap fragmentation in long-running systems)
- **How much** is allocated (wasted capacity in memory-constrained systems)

**PrototypeSTL approach:** Allocator is a template parameter with real control. Use `fixed_vector<int, 64>` for zero-allocation guarantee.

## 2. Exception-Based Error Handling

```cpp
std::vector<int> v;
v.at(999);  // throws std::out_of_range
```

Exceptions are disabled in:

- Game engines (EA, Ubisoft, Epic all use `-fno-exceptions`)
- Embedded systems (often no exception support in runtime)
- High-frequency trading (exception tables bloat icache)
- Safety-critical code (MISRA C++ forbids exceptions)

**PrototypeSTL approach:** No exceptions anywhere. Debug assertions for programmer errors. Explicit error returns for runtime failures.

## 3. Hidden Costs

| `std::` Operation | Hidden Cost |
|-------------------|-------------|
| `std::map::insert` | Heap allocation for every node |
| `std::string s = "hi"` | Possible heap allocation (no SBO guarantee) |
| `std::function<void()> f` | Possible heap allocation for captures |
| `std::shared_ptr` | Atomic reference count + control block allocation |
| `std::unordered_map` | Bucket array + node allocations |

**PrototypeSTL approach:** Every allocation is visible and controllable. `flat_map` uses contiguous storage. `string` guarantees SBO up to 22 chars. `function` has configurable inline buffer.

## 4. Poor Cache Behavior

Node-based containers (`std::map`, `std::list`, `std::unordered_map`) scatter data across the heap. On modern hardware where L1 cache miss costs 100+ cycles:

```
std::map traversal:     pointer → node → pointer → node → ...
prototype::flat_map:    [key|val|key|val|key|val|...]  (contiguous)
```

**PrototypeSTL approach:** Flat containers as the default recommendation. Cache-line-aware data layout.

## 5. Binary Size

A minimal `std::vector<int>` usage can pull in 50KB+ of template instantiations, exception handling tables, and type_info. On a microcontroller with 64KB flash, that's a non-starter.

**PrototypeSTL approach:** Minimal template depth, no exception tables, no RTTI. A `prototype::vector<int>` with pool allocator compiles to ~2KB.

## 6. The Allocator Model is Broken

`std::allocator_traits` was bolted onto the standard after the fact. In practice:

- Allocators are stateless by convention (most code assumes this)
- Propagation rules are confusing (`propagate_on_container_copy_assignment`?)
- Almost no production code uses custom allocators because it's too hard

**PrototypeSTL approach:** BDE-inspired model where allocators are stateful by design, propagation is simple, and containers actually work with non-default allocators.

## When STL is Fine

Use `std::` when:

- You're prototyping and don't care about allocation patterns
- Your platform has unlimited memory and no latency constraints
- You need full standard compliance for interop with third-party libraries
- You're writing application code, not library/infrastructure code

## When to Choose PrototypeSTL

Use PrototypeSTL when:

- You need deterministic memory behavior
- You're targeting embedded or resource-constrained platforms
- Latency spikes from allocation are unacceptable
- You compile with `-fno-exceptions -fno-rtti`
- You need control over where and how memory is allocated
- Binary size matters
