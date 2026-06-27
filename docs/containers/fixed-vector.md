# fixed_vector

## Overview

`prototype::fixed_vector<T, N>` is a vector with a compile-time maximum capacity. All storage is inline (stack or static), guaranteeing zero heap allocation. Inspired by ETL's `etl::vector` and EASTL's `fixed_vector`.

## When to Use

- Maximum size is known at compile time
- Heap allocation is forbidden (embedded, real-time, safety-critical)
- You need vector semantics with stack-allocated storage
- Interrupt handlers or signal contexts where `malloc` is unsafe

## API Reference

```cpp
namespace prototype {

template <typename T, size_t N>
class fixed_vector {
public:
    fixed_vector();
    explicit fixed_vector(size_t count, const T& value = T());

    // Element access (same as vector)
    T& operator[](size_t pos);
    T& front();
    T& back();
    T* data();

    // Capacity
    bool empty() const;
    bool full() const;          // ← unique to fixed_vector
    size_t size() const;
    static constexpr size_t capacity() { return N; }

    // Modifiers
    bool push_back(const T& value);  // returns false if full
    void pop_back();
    void clear();
    void resize(size_t count);

    // Iterators
    iterator begin();
    iterator end();
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/vector.hpp>

// 64-element sensor buffer zero heap usage
prototype::fixed_vector<float, 64> sensor_readings;

void isr_handler(float reading) {
    if (!sensor_readings.full()) {
        sensor_readings.push_back(reading);
    }
}

void process_batch() {
    for (size_t i = 0; i < sensor_readings.size(); ++i) {
        transmit(sensor_readings[i]);
    }
    sensor_readings.clear();
}
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `push_back` | O(1) | Never reallocates |
| `pop_back` | O(1) | |
| `operator[]` | O(1) | |
| `full()` | O(1) | Checks size == N |
| `clear` | O(n) | Destroys elements |

## Safety Notes

!!! warning "Capacity Overflow"
    Unlike `std::vector`, `push_back` on a full `fixed_vector` does not throw it returns `false` in release mode and asserts in debug mode.

!!! tip "Sizing"
    Choose N to be the maximum expected size plus a small margin. The full capacity is always allocated (sizeof is `N * sizeof(T) + overhead`).

## Design Notes

- Storage is a `aligned_storage<sizeof(T) * N>` no heap, no allocator
- `sizeof(fixed_vector<int, 64>)` = 260 bytes (64 ints + size counter)
- Move operations copy elements (no pointer to swap) O(n) but fast due to contiguity
- Can be used in ISR/interrupt context safely
