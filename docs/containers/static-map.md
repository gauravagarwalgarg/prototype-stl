# static_map

## Overview

`prototype::static_map<Key, Value, N>` is a fixed-capacity associative container. Like `flat_map` but with compile-time capacity bounds and zero heap allocation. Combines the cache efficiency of `flat_map` with the allocation guarantee of `fixed_vector`.

## When to Use

- Lookup tables with known maximum size
- Embedded systems where heap is unavailable
- Configuration maps built at startup
- Interrupt-safe associative access

## API Reference

```cpp
namespace prototype {

template <typename Key, typename Value, size_t N,
          typename Compare = less<Key>>
class static_map {
public:
    static_map();

    // Lookup
    Value& operator[](const Key& key);
    iterator find(const Key& key);
    bool contains(const Key& key) const;

    // Capacity
    bool empty() const;
    bool full() const;
    size_t size() const;
    static constexpr size_t capacity() { return N; }

    // Modifiers
    bool insert(const Key& key, const Value& value);  // false if full
    size_t erase(const Key& key);
    void clear();

    // Iterators
    iterator begin();
    iterator end();
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/static_map.hpp>

// Error code registry max 32 entries, zero heap
prototype::static_map<int, const char*, 32> error_messages;
error_messages.insert(1, "Timeout");
error_messages.insert(2, "Overflow");
error_messages.insert(3, "CRC Mismatch");

auto it = error_messages.find(2);
if (it != error_messages.end()) {
    log_error(it->second);
}
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `find` | O(log n) | Binary search |
| `insert` | O(n) | Shift + bounds check |
| `erase` | O(n) | Shift elements |
| `full` | O(1) | |

## Safety Notes

!!! warning "Capacity"
    `insert()` returns `false` when the map is full. In debug mode, an assertion fires. Choose N conservatively.

## Design Notes

- Storage: inline `pair<Key, Value>[N]` + size counter
- Essentially a `flat_map` on top of a `fixed_vector`
- Zero heap allocation guarantee
- `sizeof(static_map<int, int, 32>)` ≈ 260 bytes
