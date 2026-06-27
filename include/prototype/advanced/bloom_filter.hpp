///////////////////////////////////////////////////////////////////////////////
// include/prototype/advanced/bloom_filter.hpp
//
// PrototypeSTL - Probabilistic set membership test (Bloom filter)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ADVANCED_BLOOM_FILTER_HPP
#define PROTOTYPE_ADVANCED_BLOOM_FILTER_HPP

#include "../config.hpp"
#include "../containers/bitset.hpp"
#include "../functors/hash.hpp"

namespace prototype {

template<size_t N, size_t NumHashes = 3>
class bloom_filter {
public:
    typedef size_t size_type;

    bloom_filter() : item_count_(0) {}

    /// Insert a key (hashed via multiple hash functions)
    template<class Key>
    void insert(const Key& key) {
        hash<Key> h;
        size_t base = h(key);
        for (size_t i = 0; i < NumHashes; ++i) {
            size_t idx = compute_hash(base, i) % N;
            bits_.set(idx);
        }
        ++item_count_;
    }

    /// Check if key is possibly in the set (may return false positives)
    template<class Key>
    bool possibly_contains(const Key& key) const {
        hash<Key> h;
        size_t base = h(key);
        for (size_t i = 0; i < NumHashes; ++i) {
            size_t idx = compute_hash(base, i) % N;
            if (!bits_.test(idx)) return false;
        }
        return true;
    }

    /// Theoretical false positive rate: (1 - e^(-kn/m))^k
    /// where k=NumHashes, n=item_count_, m=N
    double false_positive_rate() const {
        if (item_count_ == 0) return 0.0;
        // Approximate: (bits_set / N) ^ NumHashes
        double ratio = static_cast<double>(count()) / static_cast<double>(N);
        double result = 1.0;
        for (size_t i = 0; i < NumHashes; ++i) result *= ratio;
        return result;
    }

    /// Number of bits set
    size_type count() const { return bits_.count(); }

    /// Number of items inserted
    size_type item_count() const { return item_count_; }

    /// Clear all bits
    void clear() {
        bits_.reset();
        item_count_ = 0;
    }

    constexpr size_type bit_count() const { return N; }
    constexpr size_type hash_count() const { return NumHashes; }

private:
    /// Generate i-th hash from base using double-hashing scheme
    static size_t compute_hash(size_t base, size_t i) {
        // Double hashing: h_i(x) = h1(x) + i * h2(x)
        size_t h1 = base;
        size_t h2 = base * 2654435761ULL; // Knuth's multiplicative hash
        return h1 + i * h2;
    }

    bitset<N>  bits_;
    size_type  item_count_;
};

} // namespace prototype

#endif // PROTOTYPE_ADVANCED_BLOOM_FILTER_HPP
