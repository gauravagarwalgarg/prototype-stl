# Iterators Overview

## Philosophy

Iterators are the glue between containers and algorithms. PrototypeSTL provides lightweight iterator types and utilities compatible with the standard iterator categories.

## Iterator Categories

| Category | Operations | Examples |
|----------|-----------|----------|
| InputIterator | `++`, `*`, `==`, `!=` | Stream reading |
| OutputIterator | `++`, `*` (write) | Stream writing |
| ForwardIterator | Input + multi-pass | `forward_list::iterator` |
| BidirectionalIterator | Forward + `--` | `list::iterator` |
| RandomAccessIterator | Bidirectional + `[]`, `+`, `-`, `<` | `vector::iterator`, raw pointers |

## Provided Types

### reverse_iterator

Wraps any BidirectionalIterator to traverse in reverse.

```cpp
prototype::reverse_iterator<It> rbegin(container);
prototype::reverse_iterator<It> rend(container);
```

### move_iterator

Wraps any iterator so that dereferencing produces an rvalue reference, enabling move semantics during copy algorithms.

```cpp
prototype::move_iterator<It> make_move_iterator(It it);
```

## Iterator Operations

```cpp
// Advance iterator by n positions
prototype::advance(it, n);

// Distance between two iterators
size_t d = prototype::distance(first, last);

// Next/prev (C++11 style)
auto next_it = prototype::next(it);
auto prev_it = prototype::prev(it);
```

## iterator_traits

Type introspection for iterators:

```cpp
template <typename It>
struct iterator_traits {
    using value_type = ...;
    using difference_type = ...;
    using pointer = ...;
    using reference = ...;
    using iterator_category = ...;
};
```

Specialized for raw pointers automatically.

## Example

```cpp
#include <prototype/iterators/reverse_iterator.hpp>
#include <prototype/iterators/operations.hpp>
#include <prototype/containers/vector.hpp>

prototype::vector<int> v = {1, 2, 3, 4, 5};

// Reverse iteration
for (auto it = prototype::reverse_iterator<int*>(v.end());
     it != prototype::reverse_iterator<int*>(v.begin()); ++it) {
    process(*it);  // 5, 4, 3, 2, 1
}

// Distance
auto d = prototype::distance(v.begin(), v.end());  // 5
```
