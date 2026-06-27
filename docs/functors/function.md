# function

## Overview

`prototype::function<R(Args...)>` is a type-erased callable wrapper, similar to `std::function`. It can hold function pointers, lambdas, or functors, invoking them through a uniform interface.

## When to Use

- Callbacks with unknown callable type at compile time
- Event handlers, signal/slot patterns
- Any situation requiring type-erased callables

## API Reference

```cpp
namespace prototype {

template <typename Signature>
class function;

template <typename R, typename... Args>
class function<R(Args...)> {
public:
    function();                              // Empty (null)
    function(nullptr_t);
    function(R(*func_ptr)(Args...));         // From function pointer
    
    template <typename Callable>
    function(Callable&& f);                  // From lambda/functor

    function(const function& other);
    function(function&& other);
    function& operator=(const function& other);
    function& operator=(function&& other);
    function& operator=(nullptr_t);

    // Invocation
    R operator()(Args... args) const;

    // Validity
    explicit operator bool() const;
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/functors/function.hpp>

// Hold a lambda
prototype::function<int(int, int)> add = [](int a, int b) { return a + b; };
int result = add(3, 4);  // 7

// Callbacks
using Callback = prototype::function<void(int)>;

void register_handler(Callback cb) {
    cb(42);  // Invoke
}

register_handler([](int x) { process(x); });
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Construction (small callable) | O(1) | SBO, no allocation |
| Construction (large callable) | O(1) | Heap allocation |
| `operator()` | O(1) | Indirect call via vtable-like dispatch |
| Copy | O(1) or O(n) | Depends on captured state |
| Move | O(1) | Pointer swap |

## Small Buffer Optimization

PrototypeSTL's `function` includes an inline buffer (typically 24-32 bytes) to store small callables without heap allocation. Lambdas capturing up to ~3 pointers fit inline.

| Capture Size | Storage | Allocation |
|-------------|---------|-----------|
| ≤ 24 bytes | Inline (SBO) | None |
| > 24 bytes | Heap | One allocation |
| Function pointer | Inline | None |

## Safety Notes

!!! warning "Empty function"
    Calling `operator()` on an empty (null) function is undefined behavior in release mode and asserts in debug mode.

!!! tip "Prefer templates"
    If the callable type is known at compile time, use a template parameter instead of `function`. Templates enable inlining; `function` forces an indirect call.

## Design Notes

- No RTTI used type erasure via function pointers (not `typeid`)
- No exceptions empty function triggers assertion, not `std::bad_function_call`
- SBO avoids heap allocation for most real-world callbacks
- Implementation uses a small vtable (invoke, copy, destroy function pointers)
