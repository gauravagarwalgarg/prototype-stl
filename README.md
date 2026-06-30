# PrototypeSTL

[![CI](https://github.com/gauravagarwalgarg/prototype-stl/actions/workflows/ci.yml/badge.svg)](https://github.com/gauravagarwalgarg/prototype-stl/actions/workflows/ci.yml) [![Docs](https://img.shields.io/badge/docs-live-blue?logo=github)](https://gauravagarwalgarg.github.io/prototype-stl/) ![C++](https://img.shields.io/badge/C++-17/20/23-blue?logo=cplusplus&logoColor=white) [![License](https://img.shields.io/github/license/gauravagarwalgarg/prototype-stl)](https://github.com/gauravagarwalgarg/prototype-stl/blob/main/LICENSE)

> 📖 **Documentation**: [https://gauravagarwalgarg.github.io/prototype-stl/](https://gauravagarwalgarg.github.io/prototype-stl/)
>
> 📦 **Repository**: [GitHub](https://github.com/gauravagarwalgarg/prototype-stl)


**A high-performance, embedded-friendly C++ standard library alternative.**

PrototypeSTL synthesizes the best ideas from three industry-proven libraries:

- **EASTL** (Electronic Arts) Game-engine performance patterns, custom allocator model
- **ETL** (Embedded Template Library) Fixed-capacity containers, no-heap guarantees
- **BDE** (Bloomberg) Enterprise-grade allocator architecture, component design

The result is a header-only library that works from C++11 through C++20, compiles without exceptions or RTTI, and gives you full control over memory allocation.

## Key Features

- **Zero-overhead abstractions** No virtual dispatch, no hidden allocations
- **Exception-free** Compiles with `-fno-exceptions -fno-rtti`
- **Deterministic memory** Arena, pool, and stack allocators with compile-time capacity
- **Cache-friendly** Flat containers, contiguous storage, SBO optimization
- **STL-compatible API** Drop-in replacement semantics where possible
- **Header-only** Single include path, no build step required

## Quick Start

```cpp
#include <prototype/containers/vector.hpp>
#include <prototype/allocators/arena_allocator.hpp>

prototype::memory_arena<4096> arena;
prototype::arena_allocator<int> alloc(arena);
prototype::vector<int, prototype::arena_allocator<int>> v(alloc);

v.push_back(42);
v.push_back(17);
// All allocations come from the 4KB arena zero heap usage
```

## Project Structure

```
include/prototype/     The library (header-only)
containers/            STL-compatible wrapper headers
algorithms/            Algorithm wrapper headers
iterators/             Iterator wrapper headers
allocators/            Memory/allocator wrapper headers
functors/              Functional wrapper headers
concurrency/           Atomic/concurrency wrapper headers
tests/                 Test suite
examples/              Real-world usage examples
benchmarks/            Performance benchmarks
docs/                  Documentation (mkdocs-material)
```

## Building

```bash
make              # Build everything
make tests        # Build and run tests
make examples     # Build example programs
make benchmarks   # Build benchmarks
make docs         # Serve documentation locally
make clean        # Remove build artifacts
```

## Requirements

- C++11 or later (C++14/17/20 features enabled via `PROTOTYPE_CPP_VERSION`)
- GCC 7+, Clang 6+, MSVC 2017+, or any conforming compiler
- No external dependencies

## Documentation

Full documentation is available at the [project site](https://gauravagargarg.github.io/PrototypeSTL/) or can be built locally:

```bash
pip install mkdocs-material
mkdocs serve
```

## License

MIT License. See [LICENSE](LICENSE) for details.
