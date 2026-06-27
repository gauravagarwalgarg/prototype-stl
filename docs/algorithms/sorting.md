# Sorting Algorithms

## Overview

PrototypeSTL provides a family of sorting algorithms optimized for different use cases.

## sort

The primary sorting function. Uses introsort (quicksort + heapsort fallback + insertion sort for small ranges).

```cpp
template <typename RandomIt>
void sort(RandomIt first, RandomIt last);

template <typename RandomIt, typename Compare>
void sort(RandomIt first, RandomIt last, Compare comp);
```

### Adaptive Behavior

- **Small ranges (≤16 elements):** Insertion sort (fast for nearly-sorted or small data)
- **Medium ranges:** Quicksort with median-of-three pivot
- **Recursion depth exceeded:** Switches to heapsort (guarantees O(n log n) worst case)
- **Pre-sorted detection:** Early exit when data is already sorted

### Example

```cpp
#include <prototype/algorithms/sort.hpp>
#include <prototype/functors/comparators.hpp>

prototype::vector<int> data = {5, 2, 8, 1, 9};
prototype::sort(data.begin(), data.end());
// data = {1, 2, 5, 8, 9}

// Descending
prototype::sort(data.begin(), data.end(), prototype::greater<int>());
// data = {9, 8, 5, 2, 1}
```

### Performance

| Case | Complexity | Notes |
|------|-----------|-------|
| Average | O(n log n) | Quicksort partition |
| Worst | O(n log n) | Heapsort fallback |
| Best (sorted) | O(n) | Pre-sorted detection |
| Small (≤16) | O(n²) | Insertion sort (fast in practice) |

## partial_sort

Sorts the first K elements of a range.

```cpp
template <typename RandomIt>
void partial_sort(RandomIt first, RandomIt middle, RandomIt last);
```

### Example

```cpp
prototype::vector<int> scores = {89, 42, 95, 67, 73, 88, 91};
// Get top 3
prototype::partial_sort(scores.begin(), scores.begin() + 3, scores.end(),
                        prototype::greater<int>());
// scores[0..2] = {95, 91, 89}  (rest is unspecified order)
```

**Complexity:** O(n log k) where k = middle - first

## nth_element

Places the nth element in its sorted position, partitioning the range around it.

```cpp
template <typename RandomIt>
void nth_element(RandomIt first, RandomIt nth, RandomIt last);
```

**Complexity:** O(n) average

## stable_sort

Preserves relative order of equal elements. Uses merge sort.

```cpp
template <typename RandomIt>
void stable_sort(RandomIt first, RandomIt last);
```

**Complexity:** O(n log n), uses O(n) additional space

## Safety Notes

!!! warning "Iterator validity"
    All sort algorithms require RandomAccessIterators. Passing forward or bidirectional iterators is a compile error.

!!! tip "Custom comparators"
    Comparators must define a strict weak ordering. Violating this (e.g., using `<=` instead of `<`) causes undefined behavior.
