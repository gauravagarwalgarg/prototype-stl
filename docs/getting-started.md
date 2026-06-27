# Getting Started

## Installation

PrototypeSTL is header-only. Clone the repository and add the include path to your build:

```bash
git clone https://github.com/GauravAgarwalGarg/PrototypeSTL.git
```

### CMake

```cmake
target_include_directories(my_app PRIVATE path/to/PrototypeSTL/include)
```

### Makefile

```makefile
CXXFLAGS += -Ipath/to/PrototypeSTL/include
```

### Direct Include

```cpp
#include <prototype/containers/vector.hpp>
#include <prototype/algorithms/sort.hpp>
```

## Compiler Requirements

| Compiler | Minimum Version |
|----------|----------------|
| GCC | 7.0+ |
| Clang | 6.0+ |
| MSVC | 2017 (19.14+) |
| ICC | 19.0+ |

## Build Flags

PrototypeSTL is designed to work with strict compiler settings:

```bash
g++ -std=c++11 -fno-exceptions -fno-rtti -Wall -Wextra -Wpedantic
```

## Configuration

The library auto-detects your C++ standard version. Override with:

```cpp
#define PROTOTYPE_CPP_VERSION 14  // Force C++14 features
#include <prototype/prototype.hpp>
```

### Debug Mode

```cpp
#define PROTOTYPE_DEBUG 1  // Enable bounds checking, assertions
```

## First Program

```cpp
#include <prototype/containers/vector.hpp>
#include <prototype/containers/string.hpp>
#include <prototype/algorithms/sort.hpp>
#include <cstdio>

int main() {
    prototype::vector<int> numbers;
    numbers.push_back(5);
    numbers.push_back(2);
    numbers.push_back(8);
    numbers.push_back(1);

    prototype::sort(numbers.begin(), numbers.end());

    for (size_t i = 0; i < numbers.size(); ++i) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    return 0;
}
```

Build and run:

```bash
g++ -std=c++11 -fno-exceptions -fno-rtti -Iinclude -o hello hello.cpp
./hello
# Output: 1 2 5 8
```

## Using STL-Compatible Wrappers

For drop-in STL replacement, use the wrapper headers:

```cpp
#include "containers/vector"   // Instead of <vector>
#include "algorithms/algorithm" // Instead of <algorithm>
```

These bring prototype types into the global namespace, making migration from `std::` trivial.

## Next Steps

- [Architecture](architecture.md) Understand the library structure
- [Design Philosophy](design-philosophy.md) Why we made these choices
- [Allocator Guide](allocator-guide.md) Master custom memory management
