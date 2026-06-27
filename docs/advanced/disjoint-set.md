# fixed_disjoint_set

Union-Find data structure with path compression and union by rank. All N elements are pre-allocated.

## Synopsis

```cpp
#include "prototype/advanced/disjoint_set.hpp"

template<size_t N>
class fixed_disjoint_set;
```

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `N` | Total number of elements (0 to N-1) |

## Member Functions

- `find(x)` Find root of element x (with path compression)
- `unite(x, y)` Merge sets containing x and y; returns true if they were separate
- `connected(x, y)` True if x and y are in the same set
- `count_sets()` Number of disjoint components
- `element_count()` Returns N
- `reset()` Reset all elements to individual sets

## Complexity

| Operation | Amortized |
|-----------|-----------|
| find | O(α(N)) ≈ O(1) |
| unite | O(α(N)) ≈ O(1) |
| connected | O(α(N)) ≈ O(1) |

Where α is the inverse Ackermann function (effectively constant for all practical inputs).

## Example

```cpp
prototype::fixed_disjoint_set<100> ds;

// Connect nodes in a network
ds.unite(0, 1);
ds.unite(1, 2);
ds.unite(5, 6);

// Query connectivity
ds.connected(0, 2); // true (transitive)
ds.connected(0, 5); // false

// Count components
ds.count_sets(); // 100 - 3 = 97 (merged 3 pairs, but {0,1,2} is one merge)
```

## Use Cases

- Network connectivity checking
- Kruskal's MST algorithm
- Image segmentation (connected components)
- Equivalence class tracking
