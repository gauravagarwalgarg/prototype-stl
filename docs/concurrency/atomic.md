# atomic

## Overview

`prototype::atomic<T>` provides lock-free atomic operations on scalar types. It guarantees indivisible read-modify-write operations without mutex overhead.

## When to Use

- Shared counters (statistics, reference counting)
- Flag variables (shutdown signals, ready flags)
- Lock-free algorithms (compare-and-swap loops)

## API Reference

```cpp
namespace prototype {

enum memory_order {
    memory_order_relaxed,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst
};

template <typename T>
class atomic {
public:
    atomic();
    explicit atomic(T desired);
    
    // Not copyable or movable
    atomic(const atomic&) = delete;
    atomic& operator=(const atomic&) = delete;

    // Load/Store
    T load(memory_order order = memory_order_seq_cst) const;
    void store(T desired, memory_order order = memory_order_seq_cst);

    // Exchange
    T exchange(T desired, memory_order order = memory_order_seq_cst);
    bool compare_exchange_weak(T& expected, T desired,
                               memory_order success = memory_order_seq_cst,
                               memory_order failure = memory_order_seq_cst);
    bool compare_exchange_strong(T& expected, T desired,
                                 memory_order success = memory_order_seq_cst,
                                 memory_order failure = memory_order_seq_cst);

    // Arithmetic (integer types only)
    T fetch_add(T arg, memory_order order = memory_order_seq_cst);
    T fetch_sub(T arg, memory_order order = memory_order_seq_cst);
    T fetch_and(T arg, memory_order order = memory_order_seq_cst);
    T fetch_or(T arg, memory_order order = memory_order_seq_cst);
    T fetch_xor(T arg, memory_order order = memory_order_seq_cst);

    // Operators
    T operator++();
    T operator--();
    T operator+=(T arg);
    T operator-=(T arg);
};

// Memory fences
void atomic_thread_fence(memory_order order);

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/concurrency/atomic.hpp>

// Lock-free reference counting
prototype::atomic<int> ref_count{1};

void acquire() {
    ref_count.fetch_add(1, prototype::memory_order_relaxed);
}

void release() {
    if (ref_count.fetch_sub(1, prototype::memory_order_acq_rel) == 1) {
        destroy();
    }
}

// Compare-and-swap loop (lock-free stack push)
prototype::atomic<Node*> head{nullptr};

void push(Node* node) {
    node->next = head.load(prototype::memory_order_relaxed);
    while (!head.compare_exchange_weak(node->next, node,
                                       prototype::memory_order_release,
                                       prototype::memory_order_relaxed)) {
        // Retry on failure
    }
}
```

## Memory Ordering Guide

| Order | Guarantee | Cost | Use Case |
|-------|-----------|------|----------|
| `relaxed` | Atomicity only | Lowest | Counters, statistics |
| `acquire` | Reads after this see writes before matching release | Low | Load side of publish |
| `release` | Writes before this are visible after matching acquire | Low | Store side of publish |
| `acq_rel` | Both acquire and release | Medium | Read-modify-write |
| `seq_cst` | Total global order | Highest | Default, simple correctness |

## Performance Characteristics

| Operation | x86 Cost | ARM Cost |
|-----------|----------|----------|
| `load(relaxed)` | Free (plain load) | Free |
| `load(acquire)` | Free (x86 TSO) | `ldar` instruction |
| `store(release)` | Free (x86 TSO) | `stlr` instruction |
| `store(seq_cst)` | `mfence` or `lock xchg` | `stlr` + barrier |
| `fetch_add` | `lock xadd` | `ldaxr`/`stlxr` loop |
| `compare_exchange` | `lock cmpxchg` | `ldaxr`/`stlxr` loop |

## Safety Notes

!!! warning "ABA Problem"
    `compare_exchange` is susceptible to the ABA problem in pointer-based lock-free structures. Consider tagged pointers or hazard pointers for production use.

!!! tip "Default to seq_cst"
    Use `memory_order_seq_cst` (the default) unless you have measured a performance bottleneck and understand the memory model implications of weaker orderings.

## Design Notes

- Implemented using compiler intrinsics (`__atomic_*` GCC/Clang, `_Interlocked*` MSVC)
- Supported types: integers, pointers, and any trivially-copyable type ≤ 8 bytes
- No locks, no kernel transitions
- `sizeof(atomic<int>)` = `sizeof(int)` (no overhead)
