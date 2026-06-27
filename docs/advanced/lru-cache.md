# fixed_lru_cache

A fixed-capacity Least-Recently-Used (LRU) cache. When the cache is full, the least recently accessed entry is evicted to make room for new insertions.

## Synopsis

```cpp
#include "prototype/advanced/lru_cache.hpp"

template<class Key, class Value, size_t N, class Hash = hash<Key>>
class fixed_lru_cache;
```

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `Key` | Key type |
| `Value` | Cached value type |
| `N` | Maximum number of cached entries |
| `Hash` | Hash function for keys |

## Member Functions

- `get(key)` → `optional<Value>` Retrieve value and mark as recently used
- `put(key, value)` Insert or update; evicts LRU if full
- `contains(key)` → `bool` Check without promoting
- `size()`, `capacity()`, `empty()`, `full()`
- `clear()` Remove all entries

## Internal Design

The cache combines two structures:
1. **Hash table** For O(1) key lookup
2. **Doubly-linked list** For O(1) LRU tracking (most recent at head, least recent at tail)

Both are implemented within the same fixed-capacity entry array no heap allocations.

## Example

```cpp
prototype::fixed_lru_cache<int, const char*, 3> cache;

cache.put(1, "one");
cache.put(2, "two");
cache.put(3, "three");

cache.get(1);          // promotes key 1 to most recent
cache.put(4, "four");  // evicts key 2 (least recently used)

cache.contains(2);     // false (evicted)
cache.contains(1);     // true
```

## Use Cases

- Database query caching
- DNS resolution cache
- File descriptor pool management
- Texture/asset caching in embedded graphics
- Market data snapshot caching in HFT

## Performance

| Operation | Complexity |
|-----------|-----------|
| get | O(1) average |
| put | O(1) average |
| contains | O(1) average |
| eviction | O(1) |
