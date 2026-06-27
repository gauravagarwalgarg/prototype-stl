# bitset

## Overview

`prototype::bitset<N>` is a fixed-size sequence of N bits with efficient bitwise operations. Useful for flags, bitmasks, and compact boolean arrays.

## When to Use

- Compact storage of boolean flags (1 bit per flag vs 1 byte for `bool`)
- Bitwise operations on fixed-size bit sequences
- Hardware register manipulation
- Permission masks, feature flags

## API Reference

```cpp
namespace prototype {

template <size_t N>
class bitset {
public:
    bitset();
    explicit bitset(unsigned long long val);

    // Element access
    bool operator[](size_t pos) const;
    bool test(size_t pos) const;

    // Capacity
    static constexpr size_t size() { return N; }

    // Modifiers
    bitset& set();                    // Set all bits
    bitset& set(size_t pos, bool val = true);
    bitset& reset();                  // Clear all bits
    bitset& reset(size_t pos);
    bitset& flip();                   // Toggle all bits
    bitset& flip(size_t pos);

    // Bitwise operations
    bitset operator&(const bitset& other) const;
    bitset operator|(const bitset& other) const;
    bitset operator^(const bitset& other) const;
    bitset operator~() const;
    bitset& operator&=(const bitset& other);
    bitset& operator|=(const bitset& other);
    bitset& operator^=(const bitset& other);
    bitset operator<<(size_t shift) const;
    bitset operator>>(size_t shift) const;

    // Queries
    bool all() const;
    bool any() const;
    bool none() const;
    size_t count() const;  // Population count (number of set bits)
};

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/bitset.hpp>

// Permission flags
prototype::bitset<8> permissions;
permissions.set(0);  // READ
permissions.set(1);  // WRITE
permissions.set(2);  // EXECUTE

if (permissions.test(1)) {
    // Has write permission
}

// Count active sensors
prototype::bitset<32> sensor_status(0xDEADBEEF);
size_t active = sensor_status.count();  // popcount
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `test/set/reset` | O(1) | Single word access |
| `count` | O(N/64) | Uses hardware popcount if available |
| `any/all/none` | O(N/64) | Word-level checks |
| Bitwise ops | O(N/64) | Operates on 64-bit words |
| `operator[]` | O(1) | |

## Safety Notes

!!! info "Hardware acceleration"
    `count()` uses `__builtin_popcountll` when available (GCC/Clang), falling back to a lookup table on other platforms.

## Design Notes

- Internal storage: array of `uint64_t` words, `ceil(N/64)` elements
- `sizeof(bitset<64>)` = 8 bytes, `sizeof(bitset<128>)` = 16 bytes
- All operations are constexpr where possible (C++14+)
- No dynamic allocation ever
