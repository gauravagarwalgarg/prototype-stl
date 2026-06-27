# Searching Algorithms

## Overview

Search algorithms locate elements in ranges, either linearly or using binary search on sorted data.

## find / find_if

Linear search for an element or predicate match.

```cpp
template <typename InputIt, typename T>
InputIt find(InputIt first, InputIt last, const T& value);

template <typename InputIt, typename Predicate>
InputIt find_if(InputIt first, InputIt last, Predicate pred);
```

### Example

```cpp
#include <prototype/algorithms/find.hpp>

prototype::vector<int> data = {1, 2, 3, 4, 5};

auto it = prototype::find(data.begin(), data.end(), 3);
if (it != data.end()) {
    // Found at position (it - data.begin())
}

auto even = prototype::find_if(data.begin(), data.end(),
    [](int x) { return x % 2 == 0; });
```

**Complexity:** O(n)

## binary_search

Tests whether a value exists in a sorted range.

```cpp
template <typename ForwardIt, typename T>
bool binary_search(ForwardIt first, ForwardIt last, const T& value);
```

**Complexity:** O(log n) with RandomAccessIterator, O(n) with ForwardIterator

## lower_bound / upper_bound

Find insertion points in sorted ranges.

```cpp
template <typename ForwardIt, typename T>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value);

template <typename ForwardIt, typename T>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value);
```

- `lower_bound`: First position where value could be inserted maintaining order
- `upper_bound`: Last position where value could be inserted maintaining order

### Example

```cpp
prototype::vector<int> sorted = {1, 2, 2, 3, 4, 5};

auto lb = prototype::lower_bound(sorted.begin(), sorted.end(), 2);
// Points to first '2' (index 1)

auto ub = prototype::upper_bound(sorted.begin(), sorted.end(), 2);
// Points past last '2' (index 3)
```

**Complexity:** O(log n) with RandomAccessIterator

## equal_range

Returns both lower_bound and upper_bound as a pair.

```cpp
template <typename ForwardIt, typename T>
pair<ForwardIt, ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T& value);
```

## Performance Characteristics

| Algorithm | Sorted Required? | Complexity | Iterator |
|-----------|-----------------|-----------|----------|
| `find` | No | O(n) | Input |
| `find_if` | No | O(n) | Input |
| `binary_search` | Yes | O(log n) | RandomAccess |
| `lower_bound` | Yes | O(log n) | RandomAccess |
| `upper_bound` | Yes | O(log n) | RandomAccess |

## Safety Notes

!!! warning "Sorted requirement"
    `binary_search`, `lower_bound`, and `upper_bound` require sorted input. Using them on unsorted ranges is undefined behavior.
