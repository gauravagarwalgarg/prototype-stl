# fixed_graph

Fixed-capacity adjacency-list graph supporting directed and undirected modes with BFS, DFS, and topological sort.

## Synopsis

```cpp
#include "prototype/advanced/graph.hpp"

template<size_t MaxNodes, size_t MaxEdgesPerNode, bool Directed = false>
class fixed_graph;
```

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `MaxNodes` | Maximum number of nodes |
| `MaxEdgesPerNode` | Maximum edges per node (adjacency list capacity) |
| `Directed` | `true` for directed graph, `false` for undirected |

## Member Functions

### Graph Modification
- `add_node()` → `node_type` Add a node, return its index
- `set_node_count(n)` Ensure at least n nodes exist
- `add_edge(u, v)` Add edge from u to v
- `remove_edge(u, v)` → `bool` Remove edge
- `has_edge(u, v)` → `bool`

### Queries
- `neighbors(u)` → `span<const node_type>` Adjacent nodes
- `node_count()`, `edge_count()`

### Traversal
- `bfs(start)` → `bfs_iterator` Breadth-first traversal
- `dfs(start)` → `dfs_iterator` Depth-first traversal
- `topological_sort(output, capacity)` → `size_t` Kahn's algorithm (directed only)

## Iterator Usage

```cpp
prototype::fixed_graph<16, 8, false> g;
g.set_node_count(5);
g.add_edge(0, 1);
g.add_edge(0, 2);
g.add_edge(1, 3);

// BFS
auto bfs = g.bfs(0);
while (bfs.has_next()) {
    size_t node = bfs.next();
    // process node
}
```

## Topological Sort

Only available for directed graphs (`Directed = true`). Returns the number of nodes written. If less than `node_count()`, the graph contains a cycle.

```cpp
prototype::fixed_graph<16, 8, true> dag;
dag.set_node_count(4);
dag.add_edge(0, 1);
dag.add_edge(0, 2);
dag.add_edge(1, 3);

size_t order[16];
size_t count = dag.topological_sort(order, 16);
// order = {0, 2, 1, 3} or similar valid topological ordering
```

## Panics

- `PROTOTYPE_PANIC` when adding edge beyond `MaxEdgesPerNode`
- `PROTOTYPE_PANIC` when adding node beyond `MaxNodes`
