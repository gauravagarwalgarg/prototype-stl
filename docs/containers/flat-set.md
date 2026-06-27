# flat_set

## Overview

`prototype::flat_set<Key, Compare, Allocator>` is an associative container backed by a sorted vector. It provides O(log n) lookup and membership testing with cache-friendly contiguous storage.

## When to Use

- Membership testing with good cache behavior
- Small to medium sets
- Sorted iteration required
- Drop-in for `std::set` when node allocation is too expensive

## API Reference

```cpp
namespace prototype {

template <typename Key,
          typename Compare = less<Key>,
          typename Allocator = default_allocator<Key>>
class flat_set {
public:
    flat_set();

    // Lookup
    iterator find(const Key& key);
    bool contains(const Key& key) const;
    size_t count(const Key& key) const;

    // Capacity
    bool empty() const;
    size_t size() const;

    // Modifiers
    pair<iterator, bool> insert(const Key& key);
    size_t erase(const Key& key);
    void clear();

    // Iterators (sorted order)
    iterator begin();
    iterator end();

    // Bounds
    iterator lower_bound(const Key& key);
    iterator upper_bound(const Key& key);
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/flat_set.hpp>

prototype::flat_set<int> active_ids;
active_ids.insert(42);
active_ids.insert(17);
active_ids.insert(99);

if (active_ids.contains(42)) {
    // Found
}
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `find/contains` | O(log n) | Binary search |
| `insert` | O(n) | Maintains sorted order |
| `erase` | O(n) | Shifts elements |
| Iteration | O(n) | Sequential, cache-friendly |

## Design Notes

- Backed by sorted `vector<Key>`
- Same trade-offs as `flat_map`: excellent reads, O(n) writes
- Ideal for small sets that are built once and queried many times
