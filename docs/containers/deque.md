# deque

## Overview

`prototype::deque<T, Allocator>` is a double-ended queue implemented as a block-based container. It provides O(1) insertion at both ends and O(1) random access.

## When to Use

- You need efficient push/pop at both front and back
- Random access is required (unlike `list`)
- You want better cache behavior than a linked list

## API Reference

```cpp
namespace prototype {

template <typename T, typename Allocator = default_allocator<T>>
class deque {
public:
    deque();
    explicit deque(const Allocator& alloc);

    // Element access
    T& operator[](size_t pos);
    T& front();
    T& back();

    // Capacity
    bool empty() const;
    size_t size() const;

    // Modifiers
    void push_front(const T& value);
    void push_back(const T& value);
    void pop_front();
    void pop_back();
    void clear();
    iterator insert(iterator pos, const T& value);
    iterator erase(iterator pos);

    // Iterators
    iterator begin();
    iterator end();
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/deque.hpp>

prototype::deque<int> work_queue;
work_queue.push_back(1);   // [1]
work_queue.push_back(2);   // [1, 2]
work_queue.push_front(0);  // [0, 1, 2]
work_queue.pop_front();    // [1, 2]
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `push_front` | O(1) amortized | Block allocation when full |
| `push_back` | O(1) amortized | Block allocation when full |
| `pop_front` | O(1) | |
| `pop_back` | O(1) | |
| `operator[]` | O(1) | Block index + offset calculation |
| `insert` | O(n) | Shifts within blocks |

## Safety Notes

!!! note "Block Size"
    The internal block size is tuned for cache line efficiency (typically 512 bytes per block). This is not configurable at compile time.

## Design Notes

- Implemented as an array of fixed-size blocks with a map (block pointer array)
- Iterator invalidation: inserting at front/back doesn't invalidate existing iterators (references stay valid)
- Memory: slightly higher overhead than vector due to block map, but never reallocates existing elements
