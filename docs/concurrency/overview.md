# Concurrency Overview

## Philosophy

PrototypeSTL provides low-level concurrency primitives designed for lock-free programming in latency-sensitive systems. These are not full threading abstractions they're the building blocks for writing concurrent data structures.

## Components

| Component | Description | Use Case |
|-----------|-------------|----------|
| `atomic<T>` | Lock-free atomic operations | Shared counters, flags, pointers |
| `spinlock` | Busy-wait mutual exclusion | Very short critical sections |
| `spsc_queue<T, N>` | Lock-free single-producer/single-consumer queue | Thread-to-thread messaging |

## Design Principles

1. **Lock-free where possible** No mutex, no kernel transitions
2. **Wait-free for SPSC** Both producer and consumer always make progress
3. **Memory-order explicit** No hidden sequential consistency overhead
4. **Cache-line aware** Padding to prevent false sharing

## When to Use What

```
Need shared state between threads?
├── Simple counter/flag → atomic<T>
├── Short critical section → spinlock
├── One producer, one consumer → spsc_queue
└── Multiple producers/consumers → Use std::mutex (outside our scope)
```

## Platform Support

The concurrency primitives require:

- Compiler intrinsics (`__atomic_*` on GCC/Clang, `_Interlocked*` on MSVC)
- Cache line size detection (defaults to 64 bytes)
- Memory barrier support

They work on:

- x86/x64 (all modern compilers)
- ARM (with proper memory barriers)
- RISC-V (with A extension)

## Example: Lock-Free Counter

```cpp
#include <prototype/concurrency/atomic.hpp>

prototype::atomic<int> request_count{0};

void worker_thread() {
    // Increment without locks
    request_count.fetch_add(1, prototype::memory_order_relaxed);
}

int get_count() {
    return request_count.load(prototype::memory_order_acquire);
}
```
