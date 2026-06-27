# fixed_skip_list

A probabilistic sorted data structure providing O(log n) search, insertion, and deletion. Uses a deterministic LFSR for reproducible level generation in safety-critical systems.

## Synopsis

```cpp
#include "prototype/advanced/skip_list.hpp"

template<class T, size_t N, size_t MaxLevel = 8, class Compare = /* default */>
class fixed_skip_list;
```

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `T` | Value type |
| `N` | Maximum number of elements |
| `MaxLevel` | Maximum skip list height (default: 8) |
| `Compare` | Comparison function (default: `operator<`) |

## Member Functions

- `insert(value)` → `bool` Insert element (returns false if duplicate)
- `find(value)` → `bool` Check if value exists
- `erase(value)` → `bool` Remove element
- `lower_bound(value)` → `optional<T>` First element >= value
- `size()`, `empty()`, `max_size()`

## Deterministic Randomness

Unlike standard skip lists that use system random number generators, this implementation uses a 16-bit Linear Feedback Shift Register (LFSR) for level generation. This provides:

- **Reproducibility**: Same insertion order always produces the same structure
- **No system calls**: No dependency on `/dev/random` or OS entropy
- **Constant-time level generation**: Single LFSR step per level

## Complexity

| Operation | Average | Worst Case |
|-----------|---------|------------|
| insert | O(log n) | O(n) |
| find | O(log n) | O(n) |
| erase | O(log n) | O(n) |
| lower_bound | O(log n) | O(n) |

## Example

```cpp
prototype::fixed_skip_list<int, 64> sl;

sl.insert(10);
sl.insert(5);
sl.insert(20);
sl.insert(15);

sl.find(10);  // true
sl.find(7);   // false

auto lb = sl.lower_bound(12);
// lb.has_value() && *lb == 15

sl.erase(10);
sl.find(10);  // false
```

## When to Use

| Scenario | Prefer |
|----------|--------|
| Sorted data, frequent insert/delete | skip_list |
| Static sorted data, only lookup | flat_set + binary_search |
| Unsorted, only membership test | hash_set |
| Range queries on sorted data | skip_list |
