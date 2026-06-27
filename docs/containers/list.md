# list

## Overview

`prototype::list<T, Allocator>` is a doubly-linked list supporting O(1) insertion and removal at any position, with iterator stability guarantees.

## When to Use

- You need frequent insertion/removal in the middle of a sequence
- Iterator stability is required (iterators are never invalidated except on erase)
- Splice operations between lists are needed

!!! warning "Consider flat containers first"
    In most cases, `prototype::vector` or `prototype::deque` outperforms `list` due to cache locality. Only use `list` when you genuinely need O(1) splice or stable iterators.

## API Reference

```cpp
namespace prototype {

template <typename T, typename Allocator = default_allocator<T>>
class list {
public:
    list();
    explicit list(const Allocator& alloc);

    // Element access
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
    iterator insert(iterator pos, const T& value);
    iterator erase(iterator pos);
    void clear();
    void splice(iterator pos, list& other);

    // Iterators
    iterator begin();
    iterator end();

    // Operations
    void sort();
    void reverse();
    void unique();
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/list.hpp>
#include <prototype/allocators/pool_allocator.hpp>

// Pool allocator eliminates per-node malloc overhead
prototype::pool_allocator<int> pool(1024);
prototype::list<int, prototype::pool_allocator<int>> tasks(pool);

tasks.push_back(1);
tasks.push_back(2);
tasks.push_back(3);

auto it = tasks.begin();
++it;  // points to 2
tasks.insert(it, 99);  // [1, 99, 2, 3]
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `push_front/back` | O(1) | One allocation per node |
| `pop_front/back` | O(1) | One deallocation |
| `insert` | O(1) | Given iterator position |
| `erase` | O(1) | Given iterator position |
| `splice` | O(1) | Pointer manipulation only |
| `sort` | O(n log n) | Merge sort (stable) |
| `size` | O(1) | Cached count |

## Safety Notes

!!! tip "Use pool_allocator"
    List nodes are uniform-size. A pool allocator eliminates fragmentation and reduces allocation cost from ~85ns to ~8ns per node.

## Design Notes

- Node layout: `[prev_ptr | next_ptr | T data]`
- Each node is individually allocated (use pool allocator to mitigate)
- Iterators are bidirectional
- Sort is stable (merge sort)
