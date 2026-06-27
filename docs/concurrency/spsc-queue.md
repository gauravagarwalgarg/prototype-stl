# spsc_queue

## Overview

`prototype::spsc_queue<T, N>` is a lock-free, wait-free, single-producer/single-consumer bounded queue. Both `push` and `pop` are guaranteed to complete in constant time without blocking.

## When to Use

- Thread-to-thread communication (exactly one producer, one consumer)
- Audio/video processing pipelines
- Logging (application thread → log writer thread)
- Interrupt handler → main loop communication (embedded)

## API Reference

```cpp
namespace prototype {

template <typename T, size_t N>
class spsc_queue {
public:
    spsc_queue();

    // Producer interface (call from producer thread only)
    bool push(const T& value);
    bool push(T&& value);
    template <typename... Args>
    bool emplace(Args&&... args);

    // Consumer interface (call from consumer thread only)
    bool pop(T& value);

    // Query (approximate, may be stale)
    bool empty() const;
    bool full() const;
    size_t size() const;          // Approximate
    static constexpr size_t capacity() { return N; }
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/concurrency/spsc_queue.hpp>

// 1024-element queue between producer and consumer threads
prototype::spsc_queue<int, 1024> event_queue;

// Producer thread
void producer() {
    for (int i = 0; i < 10000; ++i) {
        while (!event_queue.push(i)) {
            // Queue full back off or spin
        }
    }
}

// Consumer thread
void consumer() {
    int value;
    while (running) {
        if (event_queue.pop(value)) {
            process(value);
        }
    }
}
```

## Performance Characteristics

| Operation | Complexity | Latency | Notes |
|-----------|-----------|---------|-------|
| `push` | O(1) | ~10-20ns | Wait-free |
| `pop` | O(1) | ~10-20ns | Wait-free |
| `empty/full` | O(1) | ~5ns | Approximate |

### Throughput

| Scenario | Throughput |
|----------|-----------|
| Uncontended | 60-80M ops/sec |
| Contended (alternating) | 40-50M ops/sec |

## Safety Notes

!!! warning "Single producer, single consumer"
    This queue is ONLY safe when exactly one thread calls `push` and exactly one thread calls `pop`. Multiple producers or multiple consumers require external synchronization or a different data structure.

!!! info "Bounded capacity"
    The queue has fixed capacity N. `push` returns `false` when full (never blocks). `pop` returns `false` when empty (never blocks).

## Design Notes

- Implementation: Circular buffer with separate head (consumer) and tail (producer) indices
- Cache-line padding between head and tail to prevent false sharing
- Head and tail are atomic with acquire/release semantics (no seq_cst)
- Capacity is N (not N-1) uses a size counter or wrapping trick
- `sizeof(spsc_queue<int, 1024>)` ≈ 4KB + 128 bytes (cache-line padded indices)
- Wait-free: both push and pop always complete in bounded steps (no retry loops)
