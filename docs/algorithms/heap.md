# Heap Algorithms

## Overview

Heap algorithms maintain a max-heap property on a range, enabling O(1) access to the maximum element and O(log n) insertion/removal. The heap is stored in-place in the container (no extra allocation).

## make_heap

Converts a range into a max-heap.

```cpp
template <typename RandomIt>
void make_heap(RandomIt first, RandomIt last);

template <typename RandomIt, typename Compare>
void make_heap(RandomIt first, RandomIt last, Compare comp);
```

**Complexity:** O(n)

## push_heap

Inserts the element at `last-1` into the heap `[first, last-1)`.

```cpp
template <typename RandomIt>
void push_heap(RandomIt first, RandomIt last);
```

**Complexity:** O(log n)

## pop_heap

Moves the maximum element to position `last-1` and restores heap property on `[first, last-1)`.

```cpp
template <typename RandomIt>
void pop_heap(RandomIt first, RandomIt last);
```

**Complexity:** O(log n)

## sort_heap

Sorts a heap into ascending order (destroys the heap property).

```cpp
template <typename RandomIt>
void sort_heap(RandomIt first, RandomIt last);
```

**Complexity:** O(n log n)

## Example: Priority Queue

```cpp
#include <prototype/algorithms/heap.hpp>

prototype::vector<int> pq = {3, 1, 4, 1, 5, 9};
prototype::make_heap(pq.begin(), pq.end());
// pq[0] is now the maximum (9)

// Push new element
pq.push_back(7);
prototype::push_heap(pq.begin(), pq.end());

// Pop maximum
prototype::pop_heap(pq.begin(), pq.end());
int max_val = pq.back();  // 9
pq.pop_back();
```

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| `make_heap` | O(n) | Floyd's algorithm |
| `push_heap` | O(log n) | Sift up |
| `pop_heap` | O(log n) | Sift down |
| `sort_heap` | O(n log n) | Repeated pop |

## Safety Notes

!!! warning "Heap invariant"
    `push_heap` assumes `[first, last-1)` is already a valid heap. `pop_heap` assumes `[first, last)` is a valid heap. Violating these preconditions is UB.

## Design Notes

- Array-based binary heap: parent at `i`, children at `2i+1` and `2i+2`
- Max-heap by default; use `greater<T>` comparator for min-heap
- No extra memory allocation heap is maintained in-place
