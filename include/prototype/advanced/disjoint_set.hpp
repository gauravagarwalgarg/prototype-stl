///////////////////////////////////////////////////////////////////////////////
// include/prototype/advanced/disjoint_set.hpp
//
// PrototypeSTL - Union-Find with path compression and union by rank
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ADVANCED_DISJOINT_SET_HPP
#define PROTOTYPE_ADVANCED_DISJOINT_SET_HPP

#include "../config.hpp"

namespace prototype {

template<size_t N>
class fixed_disjoint_set {
public:
    typedef size_t size_type;

    fixed_disjoint_set() : num_sets_(N) {
        for (size_t i = 0; i < N; ++i) {
            parent_[i] = i;
            rank_[i] = 0;
        }
    }

    /// Find root of element x with path compression
    size_t find(size_t x) {
        PROTOTYPE_ASSERT(x < N);
        while (parent_[x] != x) {
            parent_[x] = parent_[parent_[x]]; // path splitting
            x = parent_[x];
        }
        return x;
    }

    /// Unite sets containing x and y. Returns true if they were in different sets.
    bool unite(size_t x, size_t y) {
        PROTOTYPE_ASSERT(x < N && y < N);
        size_t rx = find(x);
        size_t ry = find(y);
        if (rx == ry) return false;

        // Union by rank
        if (rank_[rx] < rank_[ry]) {
            parent_[rx] = ry;
        } else if (rank_[rx] > rank_[ry]) {
            parent_[ry] = rx;
        } else {
            parent_[ry] = rx;
            rank_[rx]++;
        }
        --num_sets_;
        return true;
    }

    /// Check if x and y are in the same set
    bool connected(size_t x, size_t y) {
        return find(x) == find(y);
    }

    /// Number of disjoint components
    size_type count_sets() const { return num_sets_; }

    /// Total number of elements
    constexpr size_type element_count() const { return N; }

    /// Reset all elements to individual sets
    void reset() {
        for (size_t i = 0; i < N; ++i) {
            parent_[i] = i;
            rank_[i] = 0;
        }
        num_sets_ = N;
    }

private:
    size_t    parent_[N];
    size_t    rank_[N];
    size_type num_sets_;
};

} // namespace prototype

#endif // PROTOTYPE_ADVANCED_DISJOINT_SET_HPP
