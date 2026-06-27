# Migration Guide

## Migrating from `std::` to `prototype::`

This guide covers how to incrementally replace Standard Library containers and algorithms with PrototypeSTL equivalents.

## Step 1: Include Path

Replace standard headers with PrototypeSTL headers:

| `std::` Header | PrototypeSTL Header |
|----------------|---------------------|
| `<vector>` | `<prototype/containers/vector.hpp>` |
| `<array>` | `<prototype/containers/array.hpp>` |
| `<list>` | `<prototype/containers/list.hpp>` |
| `<deque>` | `<prototype/containers/deque.hpp>` |
| `<string>` | `<prototype/containers/string.hpp>` |
| `<optional>` | `<prototype/containers/optional.hpp>` |
| `<algorithm>` | `<prototype/algorithms/sort.hpp>` etc. |
| `<numeric>` | `<prototype/algorithms/numeric.hpp>` |
| `<functional>` | `<prototype/functors/comparators.hpp>` etc. |
| `<memory>` | `<prototype/allocators/allocator.hpp>` |
| `<atomic>` | `<prototype/concurrency/atomic.hpp>` |
| `<iterator>` | `<prototype/iterators/reverse_iterator.hpp>` |

Or use the compatibility wrappers for drop-in replacement:

```cpp
#include "containers/vector"    // brings prototype::vector into scope
#include "algorithms/algorithm" // brings all algorithms into scope
```

## Step 2: Namespace Change

```cpp
// Before
std::vector<int> v;
std::sort(v.begin(), v.end());

// After
prototype::vector<int> v;
prototype::sort(v.begin(), v.end());
```

## Step 3: Exception Handling

PrototypeSTL never throws. Replace exception-based patterns:

```cpp
// Before (std::)
try {
    int val = v.at(i);
} catch (std::out_of_range&) {
    handle_error();
}

// After (prototype::)
if (i < v.size()) {
    int val = v[i];
} else {
    handle_error();
}
```

## Step 4: Allocator Migration

```cpp
// Before: default std::allocator (uncontrollable)
std::vector<int> v;

// After: explicit allocator choice
// Option A: Same behavior (malloc/free)
prototype::vector<int> v;

// Option B: Arena allocator (deterministic)
prototype::memory_arena<4096> arena;
prototype::arena_allocator<int> alloc(arena);
prototype::vector<int, prototype::arena_allocator<int>> v(alloc);

// Option C: Fixed capacity (no allocation at all)
prototype::fixed_vector<int, 256> v;
```

## API Differences

### vector

| Feature | `std::vector` | `prototype::vector` |
|---------|--------------|---------------------|
| `at()` | Throws `out_of_range` | Not provided (use `[]` + bounds check) |
| `shrink_to_fit` | Hint (may not shrink) | Actually shrinks |
| Allocator | Stateless by convention | Stateful, propagates |
| Move | O(1) | O(1) |

### string

| Feature | `std::string` | `prototype::string` |
|---------|--------------|---------------------|
| SBO threshold | Implementation-defined | 22 chars (guaranteed) |
| `c_str()` | Null-terminated | Null-terminated |
| Allocator | Standard allocator model | PrototypeSTL allocator model |
| `at()` | Throws | Not provided |

### map → flat_map

| Feature | `std::map` | `prototype::flat_map` |
|---------|-----------|----------------------|
| Storage | Red-black tree (nodes) | Sorted vector (contiguous) |
| Insert | O(log n) | O(n) |
| Lookup | O(log n) | O(log n) |
| Cache behavior | Poor (pointer chasing) | Excellent (contiguous) |
| Memory | High (per-node overhead) | Low (no overhead) |
| Iterator stability | Yes | No (insert invalidates) |

## Gradual Migration

You can use both libraries simultaneously during migration:

```cpp
#include <vector>
#include <prototype/containers/vector.hpp>

std::vector<int> old_data = get_legacy_data();

// Convert to prototype
prototype::vector<int> new_data;
new_data.reserve(old_data.size());
for (auto& item : old_data) {
    new_data.push_back(item);
}
```

## Compiler Flag Changes

```makefile
# Add -fno-exceptions -fno-rtti if not already using them
CXXFLAGS += -fno-exceptions -fno-rtti -Ipath/to/PrototypeSTL/include
```

## Common Pitfalls

1. **Don't mix `std::` and `prototype::` allocators** they're incompatible
2. **No exception safety guarantees** PrototypeSTL assumes no exceptions
3. **`at()` doesn't exist** use `[]` with manual bounds checking
4. **Allocator lifetime** ensure allocator resources outlive containers
5. **Iterator invalidation** same rules as `std::`, but flat containers invalidate more aggressively on insert
