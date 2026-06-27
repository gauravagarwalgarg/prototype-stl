# variant

## Overview

`prototype::variant<Ts...>` is a type-safe discriminated union. It holds a value from one of its alternative types at any time. Equivalent to C++17 `std::variant` but available from C++11.

## When to Use

- You need a value that can be one of several types (tagged union)
- Replacing `void*` or C-style unions with type safety
- Message types, event systems, AST nodes

## API Reference

```cpp
namespace prototype {

template <typename... Ts>
class variant {
public:
    variant();  // Default-constructs first alternative
    
    template <typename T>
    variant(T&& value);

    // Observers
    size_t index() const;          // Which alternative is active
    
    template <typename T>
    bool holds_alternative() const;
    
    template <typename T>
    T& get();
    
    template <typename T>
    const T& get() const;
    
    template <size_t I>
    auto& get();

    // Modifiers
    template <typename T, typename... Args>
    T& emplace(Args&&... args);

    // Visitation
    template <typename Visitor>
    auto visit(Visitor&& vis);
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/variant.hpp>

using Event = prototype::variant<MouseClick, KeyPress, Resize>;

Event e = MouseClick{100, 200};

if (e.holds_alternative<MouseClick>()) {
    auto& click = e.get<MouseClick>();
    handle_click(click.x, click.y);
}
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Construction | O(1) | Inline storage |
| `index` | O(1) | Returns discriminant |
| `get<T>` | O(1) | Type check + cast |
| `holds_alternative` | O(1) | Discriminant comparison |
| `visit` | O(1) | Jump table dispatch |

## Safety Notes

!!! warning "Bad access"
    Calling `get<T>()` when the variant holds a different type triggers an assertion in debug mode and is UB in release mode.

## Design Notes

- Storage: `aligned_union` of all alternative types + size_t discriminant
- `sizeof(variant<int, double, string>)` = max(sizeof alternatives) + discriminant
- No heap allocation
- Discriminant is checked at compile time for `get<T>` validity
