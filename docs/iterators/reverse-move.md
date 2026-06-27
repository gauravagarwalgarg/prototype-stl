# Reverse & Move Iterators

## reverse_iterator

### Overview

`prototype::reverse_iterator<Iterator>` adapts a bidirectional iterator to traverse a range in reverse order.

### API

```cpp
namespace prototype {

template <typename Iterator>
class reverse_iterator {
public:
    explicit reverse_iterator(Iterator base);
    
    Iterator base() const;
    
    reference operator*() const;
    pointer operator->() const;
    
    reverse_iterator& operator++();    // moves backward
    reverse_iterator& operator--();    // moves forward
    reverse_iterator operator+(difference_type n) const;
    reverse_iterator operator-(difference_type n) const;
    
    bool operator==(const reverse_iterator& other) const;
    bool operator!=(const reverse_iterator& other) const;
    bool operator<(const reverse_iterator& other) const;
};

} // namespace prototype
```

### Example

```cpp
#include <prototype/iterators/reverse_iterator.hpp>

prototype::vector<int> data = {10, 20, 30, 40, 50};

using rit = prototype::reverse_iterator<int*>;
for (rit it(data.end()); it != rit(data.begin()); ++it) {
    printf("%d ", *it);  // 50 40 30 20 10
}
```

### Design Notes

- `operator*` returns `*(base() - 1)` the base iterator points one past the element
- RandomAccessIterator operations are O(1) when base iterator supports them
- Zero overhead when inlined just reverses the increment direction

---

## move_iterator

### Overview

`prototype::move_iterator<Iterator>` wraps an iterator so dereferencing produces `T&&` instead of `T&`. This enables efficient bulk transfer using existing copy algorithms.

### API

```cpp
namespace prototype {

template <typename Iterator>
class move_iterator {
public:
    explicit move_iterator(Iterator base);
    
    Iterator base() const;
    rvalue_reference operator*() const;  // returns std::move(*base())
    
    move_iterator& operator++();
    move_iterator& operator--();
};

template <typename Iterator>
move_iterator<Iterator> make_move_iterator(Iterator it);

} // namespace prototype
```

### Example

```cpp
#include <prototype/iterators/move_iterator.hpp>
#include <prototype/algorithms/copy.hpp>

prototype::vector<prototype::string> source = {"hello", "world"};
prototype::vector<prototype::string> dest(2);

// Move all strings from source to dest (source strings left empty)
prototype::copy(
    prototype::make_move_iterator(source.begin()),
    prototype::make_move_iterator(source.end()),
    dest.begin()
);
// dest = {"hello", "world"}, source = {"", ""}
```

### When to Use

- Moving elements from a temporary container
- Transferring ownership during reallocation
- Any copy algorithm where you want move semantics instead of copy

### Performance

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `operator*` | O(1) | Cast to rvalue reference |
| `operator++/--` | O(1) | Delegates to base |
| Copy with move_iterator | O(n) | But moves instead of copies |

### Safety Notes

!!! warning "Source state"
    After using move_iterator to transfer elements, the source elements are in a valid but unspecified state. Don't read from them without reassigning first.
