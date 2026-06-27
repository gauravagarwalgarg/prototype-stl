# API Reference

## Complete Type Index

### Containers (`include/prototype/containers/`)

| Type | Header | Description |
|------|--------|-------------|
| `vector<T, A>` | `vector.hpp` | Dynamic array |
| `fixed_vector<T, N>` | `vector.hpp` | Fixed-capacity vector |
| `array<T, N>` | `array.hpp` | Fixed-size array |
| `list<T, A>` | `list.hpp` | Doubly-linked list |
| `forward_list<T, A>` | `forward_list.hpp` | Singly-linked list |
| `deque<T, A>` | `deque.hpp` | Double-ended queue |
| `string<A>` | `string.hpp` | Dynamic string with SBO |
| `fixed_string<N>` | `string.hpp` | Fixed-capacity string |
| `span<T>` | `span.hpp` | Non-owning view |
| `optional<T>` | `optional.hpp` | Nullable value |
| `variant<Ts...>` | `variant.hpp` | Discriminated union |
| `bitset<N>` | `bitset.hpp` | Fixed-size bit array |
| `flat_map<K, V, C, A>` | `flat_map.hpp` | Sorted vector map |
| `flat_set<K, C, A>` | `flat_set.hpp` | Sorted vector set |
| `ring_buffer<T, N>` | `ring_buffer.hpp` | Circular buffer |
| `static_map<K, V, N>` | `static_map.hpp` | Fixed-capacity map |
| `stack<T, C>` | `stack.hpp` | LIFO adaptor |
| `queue<T, C>` | `queue.hpp` | FIFO adaptor |

### Algorithms (`include/prototype/algorithms/`)

| Function | Header | Description |
|----------|--------|-------------|
| `sort` | `sort.hpp` | Introsort (adaptive) |
| `stable_sort` | `sort.hpp` | Merge sort |
| `partial_sort` | `sort.hpp` | Sort first K elements |
| `nth_element` | `sort.hpp` | Partition around nth |
| `find`, `find_if` | `find.hpp` | Linear search |
| `binary_search` | `find.hpp` | Sorted range search |
| `lower_bound`, `upper_bound` | `find.hpp` | Sorted range bounds |
| `copy`, `copy_if`, `copy_n` | `copy.hpp` | Range copy |
| `move` | `copy.hpp` | Move elements |
| `fill`, `fill_n` | `fill.hpp` | Fill range |
| `generate` | `fill.hpp` | Generate values |
| `transform` | `transform.hpp` | Apply function |
| `partition` | `partition.hpp` | Partition range |
| `merge` | `merge.hpp` | Merge sorted ranges |
| `rotate` | `rotate.hpp` | Rotate elements |
| `equal`, `mismatch` | `compare.hpp` | Range comparison |
| `make_heap`, `push_heap`, `pop_heap` | `heap.hpp` | Heap operations |
| `accumulate` | `numeric.hpp` | Fold/reduce |
| `inner_product` | `numeric.hpp` | Dot product |
| `partial_sum` | `numeric.hpp` | Running totals |
| `iota` | `numeric.hpp` | Sequential fill |

### Allocators (`include/prototype/allocators/`)

| Type | Header | Description |
|------|--------|-------------|
| `memory_arena<Size>` | `memory_arena.hpp` | Fixed-size memory block |
| `default_allocator<T>` | `allocator.hpp` | malloc/free wrapper |
| `arena_allocator<T>` | `arena_allocator.hpp` | Bump-pointer allocator |
| `pool_allocator<T>` | `pool_allocator.hpp` | Fixed-block free-list |
| `stack_allocator<T>` | `stack_allocator.hpp` | LIFO allocator |

### Iterators (`include/prototype/iterators/`)

| Type/Function | Header | Description |
|--------------|--------|-------------|
| `iterator_traits<It>` | `iterator_traits.hpp` | Type introspection |
| `reverse_iterator<It>` | `reverse_iterator.hpp` | Reverse adaptor |
| `move_iterator<It>` | `move_iterator.hpp` | Move-semantics adaptor |
| `advance`, `distance`, `next`, `prev` | `operations.hpp` | Iterator operations |

### Functors (`include/prototype/functors/`)

| Type | Header | Description |
|------|--------|-------------|
| `less<T>`, `greater<T>`, etc. | `comparators.hpp` | Comparison functors |
| `plus<T>`, `minus<T>`, etc. | `arithmetic.hpp` | Arithmetic functors |
| `logical_and<T>`, etc. | `logical.hpp` | Logical functors |
| `function<Sig>` | `function.hpp` | Type-erased callable |
| `hash<T>` | `hash.hpp` | Hash functions |

### Concurrency (`include/prototype/concurrency/`)

| Type | Header | Description |
|------|--------|-------------|
| `atomic<T>` | `atomic.hpp` | Lock-free atomic |
| `spinlock` | `spinlock.hpp` | Busy-wait lock |
| `spsc_queue<T, N>` | `spsc_queue.hpp` | Lock-free SPSC queue |

### Utilities (`include/prototype/utility/`)

| Function | Header | Description |
|----------|--------|-------------|
| `move(x)` | `move.hpp` | Cast to rvalue |
| `forward<T>(x)` | `forward.hpp` | Perfect forwarding |
| `swap(a, b)` | `swap.hpp` | Exchange values |
| `pair<T1, T2>` | `pair.hpp` | Pair type |

### Configuration (`include/prototype/config.hpp`)

| Macro | Description |
|-------|-------------|
| `PROTOTYPE_CPP_VERSION` | Detected C++ standard (11, 14, 17, 20) |
| `PROTOTYPE_COMPILER_GCC` | Defined if GCC |
| `PROTOTYPE_COMPILER_CLANG` | Defined if Clang |
| `PROTOTYPE_COMPILER_MSVC` | Defined if MSVC |
| `PROTOTYPE_DEBUG` | Enable debug assertions |
| `PROTOTYPE_SAFE` | Enable safe mode (no UB) |
