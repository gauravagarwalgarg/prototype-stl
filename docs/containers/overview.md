# Containers Overview

## Philosophy

PrototypeSTL containers follow these principles:

1. **Allocator-aware** Every container accepts a custom allocator template parameter
2. **Value semantics** Containers own their elements and manage lifetimes
3. **Cache-friendly defaults** Contiguous storage preferred over node-based
4. **Fixed-capacity variants** Every growable container has a fixed-size sibling

## Container Categories

### Sequence Containers

| Container | Description | Allocation |
|-----------|-------------|-----------|
| `vector<T, A>` | Dynamic array, contiguous storage | Growable |
| `fixed_vector<T, N>` | Stack-allocated vector with max capacity N | None |
| `array<T, N>` | Fixed-size array (like `std::array`) | None |
| `list<T, A>` | Doubly-linked list | Per-node |
| `deque<T, A>` | Double-ended queue, block-based | Block alloc |
| `string<A>` | Dynamic string with SBO | SBO + growable |

### Associative Containers

| Container | Description | Storage |
|-----------|-------------|---------|
| `flat_map<K, V, A>` | Sorted vector of pairs | Contiguous |
| `flat_set<K, A>` | Sorted vector of keys | Contiguous |
| `static_map<K, V, N>` | Fixed-capacity sorted map | None |

### Adaptors & Utilities

| Container | Description | Underlying |
|-----------|-------------|-----------|
| `stack<T, C>` | LIFO adaptor | deque or vector |
| `queue<T, C>` | FIFO adaptor | deque or list |
| `ring_buffer<T, N>` | Fixed-size circular buffer | Static array |
| `span<T>` | Non-owning view over contiguous data | None |
| `optional<T>` | Nullable value wrapper | Inline storage |
| `variant<Ts...>` | Type-safe union | Inline storage |
| `bitset<N>` | Fixed-size bit array | Inline storage |

## Choosing the Right Container

```
Need dynamic size?
├── Yes → Need fast random access?
│         ├── Yes → vector (or fixed_vector if max size known)
│         └── No  → Need fast insert/remove in middle?
│                   ├── Yes → list
│                   └── No  → deque
└── No  → array, bitset, ring_buffer, static_map
```

## Common API Patterns

All sequence containers provide:

```cpp
size_t size() const;
size_t capacity() const;
bool empty() const;
T& operator[](size_t i);
T& front();
T& back();
iterator begin();
iterator end();
void clear();
```

## Allocator Integration

```cpp
// Default allocator (uses malloc/free)
prototype::vector<int> v1;

// Custom allocator
prototype::memory_arena<4096> arena;
prototype::arena_allocator<int> alloc(arena);
prototype::vector<int, prototype::arena_allocator<int>> v2(alloc);

// Fixed capacity (no allocator needed)
prototype::fixed_vector<int, 100> v3;
```
