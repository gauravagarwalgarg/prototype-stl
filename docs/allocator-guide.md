# Allocator Guide

## Understanding the Allocator Architecture

PrototypeSTL's allocator model has three layers. Understanding this hierarchy is key to effective memory management.

## Layer 1: Memory Resources

Memory resources own raw bytes. They don't know about types or objects.

```cpp
// Stack-allocated arena (most common)
prototype::memory_arena<4096> arena;

// Static arena (program lifetime)
static prototype::memory_arena<65536> global_arena;
```

## Layer 2: Typed Allocators

Allocators adapt a memory resource for typed allocation.

```cpp
prototype::arena_allocator<int> alloc(arena);
int* p = alloc.allocate(10);  // 10 ints from the arena
alloc.deallocate(p, 10);       // no-op for arena_allocator
```

## Layer 3: Containers

Containers use allocators transparently.

```cpp
prototype::vector<int, prototype::arena_allocator<int>> v(alloc);
v.push_back(42);  // allocation goes through the allocator → arena
```

## Allocator Selection Guide

### arena_allocator Bulk Allocation, Bulk Free

**Pattern:** Allocate many objects, free them all at once.

```cpp
prototype::memory_arena<8192> arena;

void process_request(Request& req) {
    prototype::arena_allocator<char> alloc(arena);
    
    // All allocations during request handling
    prototype::vector<int, prototype::arena_allocator<int>> temp(alloc);
    prototype::string<prototype::arena_allocator<char>> response(alloc);
    
    // ... process ...
    
    arena.reset();  // O(1) frees everything
}
```

**Best for:** Game frames, HTTP requests, compiler phases, per-frame rendering.

### pool_allocator Uniform-Size Objects

**Pattern:** Many objects of the same size, allocated/freed individually.

```cpp
// Pool of 1000 Node objects
prototype::pool_allocator<Node> pool(1000);

void event_loop() {
    Node* n = pool.allocate(1);   // O(1)
    // ... use node ...
    pool.deallocate(n, 1);        // O(1)
}
```

**Best for:** List nodes, tree nodes, event objects, connection objects.

### stack_allocator LIFO Temporaries

**Pattern:** Nested allocations freed in reverse order.

```cpp
prototype::memory_arena<2048> scratch;
prototype::stack_allocator<float> alloc(scratch);

void compute() {
    auto mark = alloc.get_marker();
    float* buf1 = alloc.allocate(100);
    float* buf2 = alloc.allocate(200);
    
    // ... compute using buf1, buf2 ...
    
    alloc.free_to_marker(mark);  // Frees buf2 and buf1
}
```

**Best for:** Recursive algorithms, temporary buffers, scratch space.

## Sizing Arenas

### Formula

```
Arena size = (max_objects × sizeof(T)) + (alignment_overhead × max_objects)
```

### Rules of Thumb

| Use Case | Recommended Size |
|----------|-----------------|
| Small request handler | 4 KB |
| Game frame allocations | 64 KB - 1 MB |
| Compiler phase | 1 MB - 16 MB |
| Embedded sensor buffer | 256 B - 4 KB |

## Composing Allocators

Multiple allocators can share a single arena:

```cpp
prototype::memory_arena<16384> arena;

// Different typed allocators, same backing memory
prototype::arena_allocator<int> int_alloc(arena);
prototype::arena_allocator<float> float_alloc(arena);
prototype::arena_allocator<Node> node_alloc(arena);

prototype::vector<int, prototype::arena_allocator<int>> ints(int_alloc);
prototype::vector<float, prototype::arena_allocator<float>> floats(float_alloc);
prototype::list<Node, prototype::arena_allocator<Node>> nodes(node_alloc);

// All share the 16KB arena
```

## Debug Tracking

In debug mode (`PROTOTYPE_DEBUG=1`), allocators track:

- High water mark (peak usage)
- Allocation count
- Deallocation count
- Leak detection (allocations without matching deallocations)

```cpp
#ifdef PROTOTYPE_DEBUG
printf("Arena used: %zu / %zu bytes\n", arena.used(), arena.capacity());
#endif
```

## Common Mistakes

!!! warning "Dangling allocator"
    ```cpp
    prototype::vector<int, prototype::arena_allocator<int>>* make_vec() {
        prototype::memory_arena<1024> arena;  // Stack-local!
        prototype::arena_allocator<int> alloc(arena);
        auto* v = new prototype::vector<int, decltype(alloc)>(alloc);
        return v;  // BUG: arena destroyed, vector has dangling allocator
    }
    ```

!!! warning "Arena too small"
    Size your arenas for worst-case usage. An undersized arena returns nullptr, causing crashes or assertions.

!!! tip "Measure, then size"
    Use debug-mode tracking to measure actual peak usage, then set arena size to 2× peak for safety margin.
