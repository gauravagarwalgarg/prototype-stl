# fixed_hash_map

A fixed-capacity open-addressing hash map using Robin Hood hashing for excellent worst-case performance.

## Synopsis

```cpp
#include "prototype/advanced/hash_map.hpp"

template<class Key, class Value, size_t N,
         class Hash = hash<Key>,
         class KeyEqual = /* default */>
class fixed_hash_map;
```

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `Key` | Key type |
| `Value` | Mapped value type |
| `N` | Fixed capacity (number of slots) |
| `Hash` | Hash function (default: `prototype::hash<Key>`) |
| `KeyEqual` | Equality comparator (default: `operator==`) |

## Member Functions

### Capacity
- `size()` Number of stored elements
- `capacity()` Returns N
- `empty()` True if size is 0
- `full()` True if size equals N

### Lookup
- `find(key)` → `Value*` or `nullptr`
- `contains(key)` → `bool`
- `operator[](key)` → `Value&` (inserts default if absent)

### Modifiers
- `insert(key, value)` → `bool` (true if new, false if updated)
- `erase(key)` → `bool`
- `clear()`

## Robin Hood Hashing

Robin Hood hashing minimizes probe variance by "stealing" slots from entries that are closer to their ideal position. This ensures:

- O(1) average lookup
- Bounded worst-case (proportional to load factor)
- Better cache behavior than chaining

## Example

```cpp
prototype::fixed_hash_map<int, const char*, 64> cache;
cache.insert(200, "OK");
cache.insert(404, "Not Found");
cache.insert(500, "Server Error");

if (cache.contains(200)) {
    printf("Status: %s\n", *cache.find(200));
}

cache.erase(500);
```

## Performance

| Operation | Average | Worst Case |
|-----------|---------|------------|
| insert | O(1) | O(N) at high load |
| find | O(1) | O(N) at high load |
| erase | O(1) | O(N) at high load |

Keep load factor below 70-80% for optimal performance.

## Panics

- `PROTOTYPE_PANIC` on insert when map is full
- `PROTOTYPE_PANIC` on `operator[]` when map is full and key not present
