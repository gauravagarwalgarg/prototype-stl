# Benchmarks

## Methodology

All benchmarks are compiled with `-O3 -DNDEBUG` using GCC 12.2 on an Intel i7-12700K (Alder Lake). Each measurement is the median of 1000 iterations after cache warmup.

## Vector Operations

### push_back (1M integers)

| Implementation | Time (ms) | Ops/sec | Peak Memory |
|---------------|-----------|---------|-------------|
| `std::vector<int>` | 4.2 | 238M | 8.0 MB |
| `prototype::vector<int>` | 3.8 | 263M | 8.0 MB |
| `prototype::vector<int, arena>` | 2.1 | 476M | 4.0 MB |
| `prototype::fixed_vector<int, 1M>` | 1.9 | 526M | 4.0 MB |

### Random Access (1M reads)

| Implementation | Time (ns/op) | Cache Misses |
|---------------|-------------|--------------|
| `std::vector<int>` | 0.8 | 0.1% |
| `prototype::vector<int>` | 0.8 | 0.1% |
| Raw array | 0.8 | 0.1% |

!!! note
    Random access performance is identical the abstraction has zero overhead.

### Sort (1M integers, random)

| Implementation | Time (ms) | Algorithm |
|---------------|-----------|-----------|
| `std::sort` | 62 | Introsort |
| `prototype::sort` | 58 | Introsort + insertion sort |
| `prototype::sort` (presorted) | 4.1 | Adaptive detection |

## Map/Dictionary Operations

### Insert 100K key-value pairs

| Implementation | Time (ms) | Memory | Cache Behavior |
|---------------|-----------|--------|----------------|
| `std::map<int,int>` | 45 | 4.8 MB | Poor (node-based) |
| `std::unordered_map<int,int>` | 12 | 6.2 MB | Moderate |
| `prototype::flat_map<int,int>` | 8.5 | 1.6 MB | Excellent (contiguous) |
| `prototype::static_map<int,int,100K>` | 6.2 | 1.6 MB | Excellent (no alloc) |

### Lookup 100K keys (sorted)

| Implementation | Time (ms) | Complexity |
|---------------|-----------|-----------|
| `std::map<int,int>` | 18 | O(log n), pointer-chase |
| `std::unordered_map<int,int>` | 3.2 | O(1) amortized |
| `prototype::flat_map<int,int>` | 2.8 | O(log n), binary search |

## Allocator Performance

### Allocation/Deallocation Cycle (10K objects, 64 bytes each)

| Allocator | Alloc (ns/op) | Dealloc (ns/op) | Fragmentation |
|-----------|--------------|-----------------|---------------|
| `malloc/free` | 85 | 62 | High |
| `prototype::arena_allocator` | 3 | 0 (bulk free) | None |
| `prototype::pool_allocator` | 8 | 5 | None |
| `prototype::stack_allocator` | 2 | 1 (LIFO only) | None |

### Memory Arena Reset (4KB arena, 1000 allocations)

| Operation | Time |
|-----------|------|
| Individual free (malloc) | 62,000 ns |
| Arena reset (single op) | 1 ns |

## String Operations

### Short String (SSO, ≤22 chars)

| Implementation | Construction (ns) | Copy (ns) | Heap Allocs |
|---------------|-------------------|-----------|-------------|
| `std::string` | 12 | 8 | 0 (SSO) |
| `prototype::string` | 10 | 7 | 0 (SBO) |
| `prototype::fixed_string<32>` | 4 | 4 | 0 (always) |

### Long String (1KB)

| Implementation | Construction (ns) | Copy (ns) | Heap Allocs |
|---------------|-------------------|-----------|-------------|
| `std::string` | 45 | 42 | 1 |
| `prototype::string` (default) | 43 | 40 | 1 |
| `prototype::string` (arena) | 12 | 10 | 0 |

## Concurrency

### SPSC Queue Throughput (single producer, single consumer)

| Implementation | Throughput (M ops/sec) | Latency p99 (ns) |
|---------------|----------------------|-------------------|
| `std::queue` + mutex | 8 | 2400 |
| Lock-free queue (Boost) | 45 | 180 |
| `prototype::spsc_queue` | 62 | 85 |

## Binary Size

### Minimal vector program (`vector<int>`, push_back, iterate)

| Implementation | Binary Size (stripped) |
|---------------|----------------------|
| `std::vector` | 52 KB |
| `prototype::vector` | 8.4 KB |
| `prototype::fixed_vector` | 6.1 KB |

## How to Run

```bash
make benchmarks
./.dist/bench_vector
```

Or with custom parameters:

```bash
make CXXSTD=c++17 benchmarks
./.dist/bench_vector --iterations=10000
```
