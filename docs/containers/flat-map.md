# flat_map

## Overview

`prototype::flat_map<Key, Value, Compare, Allocator>` is an associative container backed by a sorted vector of key-value pairs. It provides O(log n) lookup with excellent cache behavior due to contiguous storage.

## When to Use

- Lookup-heavy workloads where cache performance matters
- Small to medium maps (under ~10K elements)
- Read-mostly access patterns (insertions are O(n) due to shifting)
- Drop-in replacement for `std::map` when node allocation overhead is unacceptable

## API Reference

```cpp
namespace prototype {

template <typename Key, typename Value,
          typename Compare = less<Key>,
          typename Allocator = default_allocator<pair<Key, Value>>>
class flat_map {
public:
    flat_map();

    // Lookup
    Value& operator[](const Key& key);
    iterator find(const Key& key);
    bool contains(const Key& key) const;
    size_t count(const Key& key) const;

    // Capacity
    bool empty() const;
    size_t size() const;

    // Modifiers
    pair<iterator, bool> insert(const pair<Key, Value>& kv);
    template <typename... Args>
    pair<iterator, bool> emplace(Args&&... args);
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
#include <prototype/containers/flat_map.hpp>

prototype::flat_map<int, const char*> status_codes;
status_codes.insert({200, "OK"});
status_codes.insert({404, "Not Found"});
status_codes.insert({500, "Internal Server Error"});

auto it = status_codes.find(404);
if (it != status_codes.end()) {
    printf("%s\n", it->second);
}
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `find` | O(log n) | Binary search on contiguous data |
| `insert` | O(n) | Shift elements to maintain order |
| `erase` | O(n) | Shift elements |
| `operator[]` | O(log n) | Find + insert if missing |
| Iteration | O(n) | Cache-friendly sequential access |
| `lower_bound` | O(log n) | Binary search |

## Safety Notes

!!! tip "Batch insert"
    If inserting many elements, insert them all then call sort, rather than inserting one-by-one (each insert shifts elements).

## Design Notes

- Stored as a single `vector<pair<Key, Value>>` kept in sorted order
- Binary search for lookup = cache-friendly (no pointer chasing like `std::map`)
- Iteration is ~5x faster than `std::map` due to contiguous layout
- Best for read-heavy workloads; `std::map` wins for write-heavy with many inserts
