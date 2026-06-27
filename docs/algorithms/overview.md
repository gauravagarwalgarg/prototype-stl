# Algorithms Overview

## Philosophy

PrototypeSTL algorithms are generic, iterator-based functions that operate on any container providing compatible iterators. They follow the same patterns as `<algorithm>` and `<numeric>` but with:

- No exception-based error reporting
- Optimized sort with adaptive detection
- Cache-aware implementations for large datasets
- Configurable comparators via functors

## Algorithm Categories

| Category | Algorithms | Header |
|----------|-----------|--------|
| Sorting | `sort`, `stable_sort`, `partial_sort`, `nth_element` | `algorithms/sort.hpp` |
| Searching | `find`, `find_if`, `binary_search`, `lower_bound`, `upper_bound` | `algorithms/find.hpp` |
| Copying | `copy`, `copy_if`, `copy_n`, `copy_backward`, `move` | `algorithms/copy.hpp` |
| Filling | `fill`, `fill_n`, `generate`, `generate_n` | `algorithms/fill.hpp` |
| Transforming | `transform`, `replace`, `replace_if` | `algorithms/transform.hpp` |
| Partitioning | `partition`, `stable_partition`, `is_partitioned` | `algorithms/partition.hpp` |
| Merging | `merge`, `inplace_merge` | `algorithms/merge.hpp` |
| Rotating | `rotate`, `rotate_copy` | `algorithms/rotate.hpp` |
| Comparing | `equal`, `mismatch`, `lexicographical_compare` | `algorithms/compare.hpp` |
| Heap | `make_heap`, `push_heap`, `pop_heap`, `sort_heap` | `algorithms/heap.hpp` |
| Numeric | `accumulate`, `inner_product`, `partial_sum`, `iota` | `algorithms/numeric.hpp` |

## Usage Pattern

```cpp
#include <prototype/algorithms/sort.hpp>
#include <prototype/algorithms/find.hpp>
#include <prototype/containers/vector.hpp>

prototype::vector<int> data = {5, 2, 8, 1, 9};

// Sort in place
prototype::sort(data.begin(), data.end());

// Binary search (requires sorted range)
auto it = prototype::lower_bound(data.begin(), data.end(), 5);

// Find with predicate
auto odd = prototype::find_if(data.begin(), data.end(),
    [](int x) { return x % 2 != 0; });
```

## Iterator Requirements

| Algorithm | Minimum Iterator Category |
|-----------|--------------------------|
| `find`, `find_if` | InputIterator |
| `copy`, `transform` | InputIterator + OutputIterator |
| `sort` | RandomAccessIterator |
| `lower_bound` | RandomAccessIterator (O(log n)) or ForwardIterator (O(n)) |
| `rotate` | ForwardIterator |
| `reverse` | BidirectionalIterator |

## Complexity Guarantees

All algorithms document their worst-case complexity. PrototypeSTL never exceeds the standard's complexity requirements and often performs better due to adaptive techniques.
