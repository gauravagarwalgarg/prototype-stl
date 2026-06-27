# optional

## Overview

`prototype::optional<T>` is a wrapper that may or may not contain a value. It provides a type-safe alternative to sentinel values, null pointers, or boolean flags. Equivalent to C++17 `std::optional` but available from C++11.

## When to Use

- A function may or may not return a value (instead of returning a sentinel)
- Optional member variables without heap allocation
- Replacing `T*` where null indicates "no value"

## API Reference

```cpp
namespace prototype {

struct nullopt_t {};
constexpr nullopt_t nullopt{};

template <typename T>
class optional {
public:
    optional();
    optional(nullopt_t);
    optional(const T& value);
    optional(T&& value);
    optional(const optional& other);
    optional(optional&& other);

    // Assignment
    optional& operator=(nullopt_t);
    optional& operator=(const T& value);
    optional& operator=(T&& value);

    // Observers
    bool has_value() const;
    explicit operator bool() const;
    T& value();
    const T& value() const;
    T& operator*();
    T* operator->();
    T value_or(T&& default_value) const;

    // Modifiers
    void reset();
    template <typename... Args>
    T& emplace(Args&&... args);
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/optional.hpp>

prototype::optional<int> find_index(const int* arr, size_t n, int target) {
    for (size_t i = 0; i < n; ++i) {
        if (arr[i] == target) return i;
    }
    return prototype::nullopt;
}

auto result = find_index(data, size, 42);
if (result.has_value()) {
    process(result.value());
}
// Or with value_or:
int idx = find_index(data, size, 42).value_or(-1);
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Construction | O(1) | Inline storage |
| `has_value` | O(1) | Boolean flag check |
| `value` | O(1) | Asserts engaged in debug |
| `reset` | O(1) | Destroys contained value |
| `emplace` | O(1) | In-place construction |

## Safety Notes

!!! warning "Accessing empty optional"
    Calling `value()` or `operator*()` on a disengaged optional is undefined behavior in release mode. In debug mode, it triggers an assertion.

## Design Notes

- Storage: `aligned_storage<sizeof(T)>` + boolean flag
- `sizeof(optional<int>)` = 8 bytes (4 for int + 4 for alignment/flag)
- No heap allocation ever
- Trivially destructible if T is trivially destructible
