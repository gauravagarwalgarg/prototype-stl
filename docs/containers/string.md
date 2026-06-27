# string

## Overview

`prototype::string<Allocator>` is a dynamic character string with Small Buffer Optimization (SBO). Short strings (≤22 chars) are stored inline without heap allocation.

## When to Use

- General-purpose string manipulation
- Drop-in replacement for `std::string`
- Need SBO guarantee (not just implementation-defined)

## API Reference

```cpp
namespace prototype {

template <typename Allocator = default_allocator<char>>
class string {
public:
    string();
    string(const char* str);
    string(const char* str, size_t len);
    string(const string& other);
    string(string&& other);

    // Element access
    char& operator[](size_t pos);
    const char* c_str() const;
    const char* data() const;

    // Capacity
    size_t size() const;
    size_t length() const;
    size_t capacity() const;
    bool empty() const;
    void reserve(size_t new_cap);

    // Modifiers
    string& operator+=(const string& str);
    string& operator+=(const char* str);
    string& operator+=(char c);
    string& append(const char* str, size_t len);
    void push_back(char c);
    void pop_back();
    void clear();
    void resize(size_t count, char c = '\0');

    // Operations
    size_t find(const char* str, size_t pos = 0) const;
    size_t find(char c, size_t pos = 0) const;
    string substr(size_t pos, size_t len = npos) const;
    int compare(const string& other) const;

    // Iterators
    iterator begin();
    iterator end();

    static const size_t npos = ~size_t(0);
};

// Also available:
template <size_t N>
class fixed_string;  // Compile-time max capacity, zero heap

} // namespace prototype
```

## Example Code

```cpp
#include <prototype/containers/string.hpp>

// SBO no heap allocation for short strings
prototype::string name = "PrototypeSTL";  // 12 chars, fits in SBO

// Long string heap allocated
prototype::string description = "A high-performance C++ standard library alternative";

// Fixed-capacity string
prototype::fixed_string<32> sensor_id = "SENSOR_0x3F";
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Construction (short) | O(n) | No allocation (SBO) |
| Construction (long) | O(n) | One allocation |
| `operator[]` | O(1) | |
| `append` | O(m) amortized | m = appended length |
| `find` | O(n*m) | Linear search |
| `c_str` | O(1) | Always null-terminated |
| `substr` | O(k) | k = substring length, allocates |

## Safety Notes

!!! info "SBO Threshold"
    Strings of 22 characters or fewer are guaranteed to be stored inline. This is part of the API contract, not an implementation detail.

!!! warning "Null termination"
    `c_str()` always returns a null-terminated string. `data()` also null-terminates (unlike pre-C++11 `std::string`).

## Design Notes

- SBO buffer: 23 bytes (22 chars + null terminator) with size stored in remaining byte
- Uses last byte as size/flag (0 = inline, else = heap pointer)
- Move from SBO string copies chars (no pointer to steal) still O(1) for short strings
- `fixed_string<N>` never allocates, asserts on overflow
