///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/bitset.hpp
//
// PrototypeSTL - Fixed-size bitset
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_BITSET_HPP
#define PROTOTYPE_BITSET_HPP

#include "../config.hpp"

namespace prototype {

template<size_t N>
class bitset {
    static const size_t BITS_PER_WORD = 8 * sizeof(unsigned long);
    static const size_t NUM_WORDS = (N + BITS_PER_WORD - 1) / BITS_PER_WORD;

public:
    bitset() { reset(); }

    // Element access
    bool test(size_t pos) const {
        PROTOTYPE_ASSERT(pos < N);
        return (words_[pos / BITS_PER_WORD] >> (pos % BITS_PER_WORD)) & 1UL;
    }

    bool operator[](size_t pos) const { return test(pos); }

    // Modifiers
    bitset& set() {
        for (size_t i = 0; i < NUM_WORDS; ++i) words_[i] = ~0UL;
        trim();
        return *this;
    }

    bitset& set(size_t pos, bool value = true) {
        PROTOTYPE_ASSERT(pos < N);
        if (value)
            words_[pos / BITS_PER_WORD] |= (1UL << (pos % BITS_PER_WORD));
        else
            words_[pos / BITS_PER_WORD] &= ~(1UL << (pos % BITS_PER_WORD));
        return *this;
    }

    bitset& reset() {
        for (size_t i = 0; i < NUM_WORDS; ++i) words_[i] = 0;
        return *this;
    }

    bitset& reset(size_t pos) {
        return set(pos, false);
    }

    bitset& flip() {
        for (size_t i = 0; i < NUM_WORDS; ++i) words_[i] = ~words_[i];
        trim();
        return *this;
    }

    bitset& flip(size_t pos) {
        PROTOTYPE_ASSERT(pos < N);
        words_[pos / BITS_PER_WORD] ^= (1UL << (pos % BITS_PER_WORD));
        return *this;
    }

    // Observers
    size_t count() const {
        size_t c = 0;
        for (size_t i = 0; i < NUM_WORDS; ++i) {
            unsigned long w = words_[i];
            while (w) { w &= (w - 1); ++c; }
        }
        return c;
    }

    constexpr size_t size() const { return N; }

    bool none() const {
        for (size_t i = 0; i < NUM_WORDS; ++i) {
            if (words_[i]) return false;
        }
        return true;
    }

    bool any() const { return !none(); }

    bool all() const { return count() == N; }

    // Bitwise operators
    bitset& operator&=(const bitset& other) {
        for (size_t i = 0; i < NUM_WORDS; ++i) words_[i] &= other.words_[i];
        return *this;
    }

    bitset& operator|=(const bitset& other) {
        for (size_t i = 0; i < NUM_WORDS; ++i) words_[i] |= other.words_[i];
        return *this;
    }

    bitset& operator^=(const bitset& other) {
        for (size_t i = 0; i < NUM_WORDS; ++i) words_[i] ^= other.words_[i];
        return *this;
    }

    bitset operator~() const {
        bitset result(*this);
        result.flip();
        return result;
    }

private:
    void trim() {
        if (N % BITS_PER_WORD != 0) {
            words_[NUM_WORDS - 1] &= (1UL << (N % BITS_PER_WORD)) - 1;
        }
    }

    unsigned long words_[NUM_WORDS];
};

} // namespace prototype

#endif // PROTOTYPE_BITSET_HPP
