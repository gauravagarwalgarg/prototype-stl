# fixed_trie

A prefix tree (trie) with all nodes from a fixed pool. Useful for routing tables, IP lookup, command parsing, and string matching.

## Synopsis

```cpp
#include "prototype/advanced/trie.hpp"

template<size_t MaxNodes, size_t AlphabetSize = 26>
class fixed_trie;
```

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `MaxNodes` | Maximum number of nodes in the trie |
| `AlphabetSize` | Size of the character alphabet (default: 26 for lowercase English) |

## Character Mapping

The default mapping handles:
- `'a'`–`'z'` → indices 0–25
- `'A'`–`'Z'` → indices 0–25 (case-insensitive)
- `'0'`–`'9'` → indices 0–9

For custom alphabets, extend or modify the `char_to_index` function.

## Member Functions

- `insert(key, len)` Insert a string key
- `search(key, len)` → `bool` Exact match lookup
- `starts_with(prefix, len)` → `bool` Prefix existence check
- `erase(key, len)` → `bool` Remove a key
- `size()` Number of nodes allocated
- `clear()` Reset the trie

## Example

```cpp
prototype::fixed_trie<256, 26> trie;

trie.insert("route", 5);
trie.insert("router", 6);
trie.insert("routing", 7);

trie.search("route", 5);       // true
trie.search("rout", 4);        // false (not a complete word)
trie.starts_with("rou", 3);    // true
trie.starts_with("xyz", 3);    // false

trie.erase("route", 5);
trie.search("route", 5);       // false
trie.search("router", 6);      // true (still exists)
```

## Memory Usage

Each node uses `AlphabetSize * sizeof(size_t) + sizeof(bool)` bytes. For AlphabetSize=26 on 64-bit:
- Per node: ~209 bytes
- 256 nodes: ~53 KB

Choose `MaxNodes` based on expected total string length and branching factor.

## Use Cases

- Command-line autocompletion
- IP routing table (with AlphabetSize=2 for binary trie)
- DNS resolution caching
- Dictionary spell-checking
