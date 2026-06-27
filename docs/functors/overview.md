# Functors Overview

## Philosophy

Functors (function objects) are callable objects that encapsulate operations. PrototypeSTL provides a complete set of comparators, arithmetic operations, and a type-erased `function` wrapper all working without exceptions or RTTI.

## Categories

| Category | Components | Header |
|----------|-----------|--------|
| Comparators | `less`, `greater`, `equal_to`, `not_equal_to`, `less_equal`, `greater_equal` | `functors/comparators.hpp` |
| Arithmetic | `plus`, `minus`, `multiplies`, `divides`, `modulus`, `negate` | `functors/arithmetic.hpp` |
| Logical | `logical_and`, `logical_or`, `logical_not` | `functors/logical.hpp` |
| Callable | `function<Sig>` | `functors/function.hpp` |
| Hashing | `hash<T>` | `functors/hash.hpp` |

## Usage Pattern

```cpp
#include <prototype/functors/comparators.hpp>
#include <prototype/algorithms/sort.hpp>

prototype::vector<int> data = {5, 2, 8, 1};

// Sort descending using greater<> functor
prototype::sort(data.begin(), data.end(), prototype::greater<int>());

// Sort ascending (default)
prototype::sort(data.begin(), data.end(), prototype::less<int>());
```

## Why Functors Over Function Pointers?

| Feature | Function Pointer | Functor |
|---------|-----------------|---------|
| Inlining | Rarely inlined | Always inlined |
| State | Cannot hold state | Can capture state |
| Optimization | Indirect call | Direct call (zero overhead) |
| Size | 8 bytes (pointer) | 1 byte (empty base optimization) |

The compiler sees the functor type at the call site, enabling complete inlining. A function pointer creates an indirect call that blocks optimization.
