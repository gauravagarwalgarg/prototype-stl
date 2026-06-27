# Numeric Algorithms

## Overview

Numeric algorithms perform mathematical operations over ranges, equivalent to `<numeric>`.

## accumulate

Computes the sum (or general fold) of a range.

```cpp
template <typename InputIt, typename T>
T accumulate(InputIt first, InputIt last, T init);

template <typename InputIt, typename T, typename BinaryOp>
T accumulate(InputIt first, InputIt last, T init, BinaryOp op);
```

### Example

```cpp
#include <prototype/algorithms/numeric.hpp>

prototype::vector<int> values = {1, 2, 3, 4, 5};

int sum = prototype::accumulate(values.begin(), values.end(), 0);
// sum = 15

int product = prototype::accumulate(values.begin(), values.end(), 1,
    [](int a, int b) { return a * b; });
// product = 120
```

## inner_product

Computes the dot product of two ranges.

```cpp
template <typename InputIt1, typename InputIt2, typename T>
T inner_product(InputIt1 first1, InputIt1 last1, InputIt2 first2, T init);
```

### Example

```cpp
prototype::vector<float> a = {1.0f, 2.0f, 3.0f};
prototype::vector<float> b = {4.0f, 5.0f, 6.0f};

float dot = prototype::inner_product(a.begin(), a.end(), b.begin(), 0.0f);
// dot = 1*4 + 2*5 + 3*6 = 32.0
```

## partial_sum

Computes running totals.

```cpp
template <typename InputIt, typename OutputIt>
OutputIt partial_sum(InputIt first, InputIt last, OutputIt d_first);
```

### Example

```cpp
prototype::vector<int> data = {1, 2, 3, 4, 5};
prototype::vector<int> result(5);
prototype::partial_sum(data.begin(), data.end(), result.begin());
// result = {1, 3, 6, 10, 15}
```

## iota

Fills a range with sequentially increasing values.

```cpp
template <typename ForwardIt, typename T>
void iota(ForwardIt first, ForwardIt last, T value);
```

### Example

```cpp
prototype::vector<int> indices(10);
prototype::iota(indices.begin(), indices.end(), 0);
// indices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
```

## Performance Characteristics

| Algorithm | Complexity | Notes |
|-----------|-----------|-------|
| `accumulate` | O(n) | Single pass |
| `inner_product` | O(n) | Two parallel reads |
| `partial_sum` | O(n) | Single pass, writes output |
| `iota` | O(n) | Sequential writes |

## Safety Notes

!!! tip "Overflow"
    `accumulate` and `inner_product` use the init type for the running total. Use a wider type (e.g., `int64_t`) if overflow is possible with `int`.
