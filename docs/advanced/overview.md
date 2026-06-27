# Advanced Data Structures

PrototypeSTL's advanced data structures are built on top of the core containers and provide sophisticated functionality for demanding applications. All structures use **fixed-capacity, stack-allocated storage** no heap allocation, no exceptions, no RTTI.

## Design Principles

- **Fixed capacity**: Template parameter `N` determines maximum size at compile time
- **Zero allocation**: All storage is embedded in the object
- **Deterministic**: No hidden allocations or system calls in the critical path
- **Panic on overflow**: `PROTOTYPE_PANIC` when capacity is exceeded (configurable handler)

## Available Structures

| Structure | Header | Use Case |
|-----------|--------|----------|
| `fixed_hash_map` | `advanced/hash_map.hpp` | O(1) key-value lookup |
| `fixed_hash_set` | `advanced/hash_set.hpp` | O(1) membership test |
| `fixed_disjoint_set` | `advanced/disjoint_set.hpp` | Union-Find, connectivity |
| `fixed_graph` | `advanced/graph.hpp` | Adjacency-list graph with BFS/DFS |
| `bloom_filter` | `advanced/bloom_filter.hpp` | Probabilistic membership |
| `fixed_trie` | `advanced/trie.hpp` | Prefix tree for string keys |
| `fixed_skip_list` | `advanced/skip_list.hpp` | Sorted list with O(log n) search |
| `fixed_lru_cache` | `advanced/lru_cache.hpp` | Least-recently-used eviction |
| `fixed_circular_buffer` | `advanced/circular_buffer.hpp` | Overwrite-oldest ring buffer |
| `fixed_priority_queue` | `advanced/priority_queue.hpp` | Binary heap |

## Usage

```cpp
#include "prototype/prototype.hpp"
// or include individually:
#include "prototype/advanced/hash_map.hpp"
```

## Comparison with Basic Containers

| Need | Basic | Advanced |
|------|-------|----------|
| Key lookup | `flat_map` (O(log n)) | `fixed_hash_map` (O(1)) |
| Sorted insert | `flat_set` | `fixed_skip_list` |
| Priority access | manual with `heap` algorithms | `fixed_priority_queue` |
| Connectivity | manual | `fixed_disjoint_set` |
| Path finding | manual | `fixed_graph` with BFS/DFS |
