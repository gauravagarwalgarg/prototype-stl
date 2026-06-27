# ring_buffer

## Overview

`prototype::ring_buffer<T, N>` is a fixed-capacity circular buffer. When full, new elements overwrite the oldest. Ideal for streaming data, logging, and producer-consumer patterns.

## When to Use

- Fixed-size sliding window over streaming data
- Log buffers that keep the last N entries
- Audio/video sample buffers
- Sensor data where only recent readings matter

## API Reference

```cpp
namespace prototype {

template <typename T, size_t N>
class ring_buffer {
public:
    ring_buffer();

    // Element access
    T& front();           // Oldest element
    T& back();            // Newest element
    T& operator[](size_t i);  // i=0 is oldest

    // Capacity
    bool empty() const;
    bool full() const;
    size_t size() const;
    static constexpr size_t capacity() { return N; }

    // Modifiers
    void push_back(const T& value);   // Overwrites oldest if full
    void pop_front();                  // Remove oldest
    void clear();

    // Iterators
    iterator begin();
    iterator end();
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/ring_buffer.hpp>

// Keep last 128 sensor readings
prototype::ring_buffer<float, 128> readings;

void on_sensor_data(float value) {
    readings.push_back(value);  // Oldest drops off when full
}

float average() {
    float sum = 0;
    for (size_t i = 0; i < readings.size(); ++i) {
        sum += readings[i];
    }
    return sum / readings.size();
}
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `push_back` | O(1) | Always, even when full (overwrites) |
| `pop_front` | O(1) | |
| `operator[]` | O(1) | Modular index calculation |
| `front/back` | O(1) | |
| `size` | O(1) | |

## Safety Notes

!!! info "Overwrite behavior"
    `push_back` on a full ring buffer overwrites the oldest element without warning. Check `full()` first if you need to detect overflow.

## Design Notes

- Storage: `T[N]` with head and tail indices
- No heap allocation ever
- `sizeof(ring_buffer<int, 128>)` = 520 bytes (512 for data + indices)
- Ideal for ISR/interrupt contexts
- Iterator wraps around transparently
