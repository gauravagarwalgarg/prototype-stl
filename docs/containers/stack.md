# stack

## Overview

`prototype::stack<T, Container>` is a LIFO (Last-In, First-Out) container adaptor. It wraps an underlying container (default: `deque`) and exposes only stack operations.

## When to Use

- Expression evaluation, undo systems
- DFS traversal without recursion
- Any LIFO access pattern

## API Reference

```cpp
namespace prototype {

template <typename T, typename Container = deque<T>>
class stack {
public:
    stack();

    T& top();
    const T& top() const;
    bool empty() const;
    size_t size() const;
    void push(const T& value);
    void push(T&& value);
    void pop();
    void swap(stack& other);
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/stack.hpp>

prototype::stack<int> undo_stack;
undo_stack.push(save_state());
// ... do work ...
int prev = undo_stack.top();
undo_stack.pop();
restore_state(prev);
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `push` | O(1) amortized | Delegates to underlying container |
| `pop` | O(1) | |
| `top` | O(1) | |

## Design Notes

- Thin adaptor all performance depends on the underlying container
- Use `stack<T, vector<T>>` for contiguous storage
- Use `stack<T, fixed_vector<T, N>>` for zero-allocation stack
