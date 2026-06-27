# Copying Algorithms

## Overview

Copy algorithms transfer elements between ranges without modifying the source.

## copy

```cpp
template <typename InputIt, typename OutputIt>
OutputIt copy(InputIt first, InputIt last, OutputIt d_first);

template <typename InputIt, typename OutputIt, typename Predicate>
OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, Predicate pred);

template <typename InputIt, typename Size, typename OutputIt>
OutputIt copy_n(InputIt first, Size count, OutputIt d_first);

template <typename BidirIt1, typename BidirIt2>
BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last);
```

## Example

```cpp
#include <prototype/algorithms/copy.hpp>

prototype::vector<int> src = {1, 2, 3, 4, 5};
prototype::vector<int> dst(5);

prototype::copy(src.begin(), src.end(), dst.begin());

// Copy only even numbers
prototype::vector<int> evens(5);
auto end = prototype::copy_if(src.begin(), src.end(), evens.begin(),
    [](int x) { return x % 2 == 0; });
// evens contains {2, 4}
```

## move

Transfers elements using move semantics (source left in valid but unspecified state).

```cpp
template <typename InputIt, typename OutputIt>
OutputIt move(InputIt first, InputIt last, OutputIt d_first);
```

## Performance Characteristics

| Algorithm | Complexity | Notes |
|-----------|-----------|-------|
| `copy` | O(n) | May use `memcpy` for trivially-copyable types |
| `copy_if` | O(n) | Conditional copy |
| `copy_n` | O(n) | Exactly n copies |
| `copy_backward` | O(n) | Safe for overlapping ranges (right overlap) |
| `move` | O(n) | May use `memmove` for trivial types |

## Safety Notes

!!! tip "Trivial type optimization"
    For trivially-copyable types, `copy` is optimized to `memcpy` (non-overlapping) or `memmove` (overlapping). This happens automatically.

!!! warning "Overlapping ranges"
    Use `copy_backward` when source and destination overlap with destination after source. Use `copy` when destination is before source or ranges don't overlap.
