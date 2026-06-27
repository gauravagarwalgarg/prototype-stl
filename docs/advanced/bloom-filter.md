# bloom_filter

A space-efficient probabilistic data structure for set membership testing. May return false positives but never false negatives.

## Synopsis

```cpp
#include "prototype/advanced/bloom_filter.hpp"

template<size_t N, size_t NumHashes = 3>
class bloom_filter;
```

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `N` | Number of bits in the filter |
| `NumHashes` | Number of hash functions (default: 3) |

## Member Functions

- `insert(key)` Add an element to the filter
- `possibly_contains(key)` → `bool` Check membership (may false-positive)
- `false_positive_rate()` → `double` Approximate current FPR
- `count()` Number of bits set
- `item_count()` Number of items inserted
- `clear()` Reset all bits
- `bit_count()` Returns N
- `hash_count()` Returns NumHashes

## How It Works

A bloom filter uses K independent hash functions to map each element to K bit positions. On insert, those K bits are set. On query, if all K bits are set, the element is "possibly present".

**Key properties:**
- No false negatives: if `possibly_contains` returns false, the element was definitely never inserted
- False positives: if it returns true, the element *might* have been inserted
- Cannot remove elements (use counting bloom filter for that)

## Sizing Guide

For a target false positive rate `p` with `n` expected elements:
- Optimal bits: `m = -n * ln(p) / (ln(2)^2)`
- Optimal hashes: `k = (m/n) * ln(2)`

| Elements | FPR Target | Bits Needed | Hashes |
|----------|------------|-------------|--------|
| 100 | 1% | 958 | 7 |
| 1000 | 1% | 9585 | 7 |
| 100 | 0.1% | 1437 | 10 |

## Example

```cpp
prototype::bloom_filter<1024, 5> filter;

filter.insert(42);
filter.insert(100);

filter.possibly_contains(42);  // true (definitely)
filter.possibly_contains(999); // probably false, but might be true
```

## Use Cases

- Cache hit pre-filtering
- Network packet deduplication
- Spell checking (word existence)
- Database query optimization
