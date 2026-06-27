# hash

## Overview

`prototype::hash<T>` provides hash function implementations for fundamental and common types. Used by hash-based containers and general hashing needs.

## When to Use

- Hash table implementations
- Data deduplication
- Fast equality pre-check
- Custom hash containers

## API Reference

```cpp
namespace prototype {

template <typename T>
struct hash;

// Specializations provided:
template <> struct hash<int>;
template <> struct hash<unsigned int>;
template <> struct hash<long>;
template <> struct hash<unsigned long>;
template <> struct hash<long long>;
template <> struct hash<unsigned long long>;
template <> struct hash<float>;
template <> struct hash<double>;
template <> struct hash<const char*>;
template <> struct hash<prototype::string>;
template <typename T> struct hash<T*>;

// Usage
template <typename T>
struct hash {
    size_t operator()(const T& value) const;
};

// Combine hashes (for composite keys)
size_t hash_combine(size_t seed, size_t hash);

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/functors/hash.hpp>

prototype::hash<int> int_hasher;
size_t h = int_hasher(42);

// Composite key hashing
struct Point { int x, y; };

struct PointHash {
    size_t operator()(const Point& p) const {
        prototype::hash<int> h;
        size_t seed = h(p.x);
        return prototype::hash_combine(seed, h(p.y));
    }
};
```

## Hash Quality

The hash functions use:

- **Integers:** Multiplicative hash (fast, good distribution)
- **Floating point:** Bit-cast to integer, then hash (handles -0.0 == +0.0)
- **Strings:** FNV-1a (good distribution, fast for short strings)
- **Pointers:** Shift and multiply (removes low-bit alignment patterns)

## Performance Characteristics

| Type | Time (ns) | Quality |
|------|-----------|---------|
| `int` | 1-2 | Good (multiplicative) |
| `const char*` (short) | 5-10 | Excellent (FNV-1a) |
| `const char*` (long) | O(n) | Excellent |
| `double` | 2-3 | Good |
| `pointer` | 1-2 | Good |

## Safety Notes

!!! info "Determinism"
    Hash values are deterministic within a single program run. They are NOT guaranteed stable across different compilations, platforms, or library versions. Do not persist hash values.

## Design Notes

- No randomization (unlike some `std::hash` implementations)
- `hash_combine` uses the Boost formula: `seed ^= hash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2)`
- Custom types: specialize `prototype::hash<YourType>` or provide a callable
