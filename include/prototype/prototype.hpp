///////////////////////////////////////////////////////////////////////////////
// include/prototype/prototype.hpp
//
// PrototypeSTL - Master header (includes all components)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_MASTER_HPP
#define PROTOTYPE_MASTER_HPP

// Configuration
#include "config.hpp"

// Type traits
#include "type_traits/type_traits.hpp"

// Utility
#include "utility/utility.hpp"

// Iterators
#include "iterators/iterator_traits.hpp"
#include "iterators/reverse_iterator.hpp"
#include "iterators/move_iterator.hpp"
#include "iterators/operations.hpp"

// Allocators
#include "allocators/memory_arena.hpp"
#include "allocators/allocator.hpp"
#include "allocators/arena_allocator.hpp"
#include "allocators/pool_allocator.hpp"
#include "allocators/stack_allocator.hpp"

// Algorithms
#include "algorithms/copy.hpp"
#include "algorithms/fill.hpp"
#include "algorithms/find.hpp"
#include "algorithms/sort.hpp"
#include "algorithms/transform.hpp"
#include "algorithms/compare.hpp"
#include "algorithms/heap.hpp"
#include "algorithms/numeric.hpp"
#include "algorithms/partition.hpp"
#include "algorithms/rotate.hpp"
#include "algorithms/merge.hpp"
#include "algorithms/binary_search.hpp"
#include "algorithms/minmax.hpp"
#include "algorithms/set_operations.hpp"

// Containers
#include "containers/array.hpp"
#include "containers/vector.hpp"
#include "containers/list.hpp"
#include "containers/deque.hpp"
#include "containers/string.hpp"
#include "containers/span.hpp"
#include "containers/optional.hpp"
#include "containers/bitset.hpp"
#include "containers/forward_list.hpp"
#include "containers/stack.hpp"
#include "containers/queue.hpp"
#include "containers/variant.hpp"
#include "containers/flat_set.hpp"
#include "containers/flat_map.hpp"
#include "containers/ring_buffer.hpp"
#include "containers/static_map.hpp"

// Functors
#include "functors/comparators.hpp"
#include "functors/arithmetic.hpp"
#include "functors/logical.hpp"
#include "functors/function.hpp"
#include "functors/hash.hpp"

// Concurrency
#include "concurrency/atomic.hpp"
#include "concurrency/spinlock.hpp"
#include "concurrency/spsc_queue.hpp"

// Core
#include "core/tuple.hpp"
#include "core/numeric_limits.hpp"
#include "core/ratio.hpp"
#include "core/chrono.hpp"
#include "core/any.hpp"
#include "core/expected.hpp"
#include "core/string_view.hpp"

// Advanced Data Structures
#include "advanced/hash_map.hpp"
#include "advanced/hash_set.hpp"
#include "advanced/disjoint_set.hpp"
#include "advanced/graph.hpp"
#include "advanced/bloom_filter.hpp"
#include "advanced/trie.hpp"
#include "advanced/skip_list.hpp"
#include "advanced/lru_cache.hpp"
#include "advanced/circular_buffer.hpp"
#include "advanced/priority_queue.hpp"

#endif // PROTOTYPE_MASTER_HPP
