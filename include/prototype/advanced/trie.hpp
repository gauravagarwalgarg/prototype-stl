///////////////////////////////////////////////////////////////////////////////
// include/prototype/advanced/trie.hpp
//
// PrototypeSTL - Fixed-capacity prefix tree (trie)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ADVANCED_TRIE_HPP
#define PROTOTYPE_ADVANCED_TRIE_HPP

#include "../config.hpp"

namespace prototype {

template<size_t MaxNodes, size_t AlphabetSize = 26>
class fixed_trie {
public:
    typedef size_t size_type;

    fixed_trie() : node_count_(1) {
        // Initialize root node (index 0)
        clear_node(0);
    }

    /// Insert a key of given length. Characters mapped to indices [0, AlphabetSize).
    void insert(const char* key, size_t len) {
        size_t cur = 0;
        for (size_t i = 0; i < len; ++i) {
            size_t c = char_to_index(key[i]);
            PROTOTYPE_ASSERT(c < AlphabetSize);
            if (nodes_[cur].children[c] == INVALID) {
                PROTOTYPE_ASSERT(node_count_ < MaxNodes);
                size_t new_node = node_count_++;
                clear_node(new_node);
                nodes_[cur].children[c] = new_node;
            }
            cur = nodes_[cur].children[c];
        }
        nodes_[cur].is_end = true;
    }

    /// Check if exact key exists
    bool search(const char* key, size_t len) const {
        size_t cur = 0;
        for (size_t i = 0; i < len; ++i) {
            size_t c = char_to_index(key[i]);
            if (c >= AlphabetSize) return false;
            if (nodes_[cur].children[c] == INVALID) return false;
            cur = nodes_[cur].children[c];
        }
        return nodes_[cur].is_end;
    }

    /// Check if any word starts with this prefix
    bool starts_with(const char* prefix, size_t len) const {
        size_t cur = 0;
        for (size_t i = 0; i < len; ++i) {
            size_t c = char_to_index(prefix[i]);
            if (c >= AlphabetSize) return false;
            if (nodes_[cur].children[c] == INVALID) return false;
            cur = nodes_[cur].children[c];
        }
        return true;
    }

    /// Erase a key. Returns true if the key was present.
    bool erase(const char* key, size_t len) {
        return erase_helper(0, key, len, 0);
    }

    /// Number of nodes allocated
    size_type size() const { return node_count_; }

    /// Reset trie to empty state
    void clear() {
        node_count_ = 1;
        clear_node(0);
    }

private:
    static const size_t INVALID = static_cast<size_t>(-1);

    struct node {
        size_t children[AlphabetSize];
        bool   is_end;
    };

    void clear_node(size_t idx) {
        for (size_t i = 0; i < AlphabetSize; ++i) {
            nodes_[idx].children[i] = INVALID;
        }
        nodes_[idx].is_end = false;
    }

    static size_t char_to_index(char c) {
        // Default mapping: lowercase 'a'-'z' -> 0-25
        if (c >= 'a' && c <= 'z') return static_cast<size_t>(c - 'a');
        if (c >= 'A' && c <= 'Z') return static_cast<size_t>(c - 'A');
        if (c >= '0' && c <= '9') return static_cast<size_t>(c - '0');
        return AlphabetSize; // invalid
    }

    bool has_children(size_t idx) const {
        for (size_t i = 0; i < AlphabetSize; ++i) {
            if (nodes_[idx].children[i] != INVALID) return true;
        }
        return false;
    }

    bool erase_helper(size_t cur, const char* key, size_t len, size_t depth) {
        if (depth == len) {
            if (!nodes_[cur].is_end) return false;
            nodes_[cur].is_end = false;
            return true;
        }
        size_t c = char_to_index(key[depth]);
        if (c >= AlphabetSize) return false;
        if (nodes_[cur].children[c] == INVALID) return false;

        bool deleted = erase_helper(nodes_[cur].children[c], key, len, depth + 1);

        if (deleted) {
            size_t child = nodes_[cur].children[c];
            if (!nodes_[child].is_end && !has_children(child)) {
                nodes_[cur].children[c] = INVALID;
                // Note: we don't actually reclaim the node in fixed pool
            }
        }
        return deleted;
    }

    node      nodes_[MaxNodes];
    size_type node_count_;
};

} // namespace prototype

#endif // PROTOTYPE_ADVANCED_TRIE_HPP
