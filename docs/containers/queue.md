# queue

## Overview

`prototype::queue<T, Container>` is a FIFO (First-In, First-Out) container adaptor. It wraps an underlying container (default: `deque`) and exposes only queue operations.

## When to Use

- Task scheduling, message passing
- BFS traversal
- Producer-consumer patterns (single-threaded)

## API Reference

```cpp
namespace prototype {

template <typename T, typename Container = deque<T>>
class queue {
public:
    queue();

    T& front();
    T& back();
    bool empty() const;
    size_t size() const;
    void push(const T& value);
    void push(T&& value);
    void pop();
    void swap(queue& other);
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/queue.hpp>

prototype::queue<int> task_queue;
task_queue.push(1);
task_queue.push(2);

while (!task_queue.empty()) {
    int task = task_queue.front();
    task_queue.pop();
    execute(task);
}
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `push` | O(1) amortized | |
| `pop` | O(1) | |
| `front/back` | O(1) | |

## Design Notes

- For multi-threaded producer-consumer, use `spsc_queue` instead
- Use `ring_buffer` for fixed-capacity FIFO with overwrite semantics
