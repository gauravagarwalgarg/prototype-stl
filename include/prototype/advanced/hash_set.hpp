///////////////////////////////////////////////////////////////////////////////
// include/prototype/advanced/hash_set.hpp
//
// PrototypeSTL - Fixed-capacity open-addressing hash set with Robin Hood hashing
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ADVANCED_HASH_SET_HPP
#define PROTOTYPE_ADVANCED_HASH_SET_HPP

#include "../config.hpp"
#include "../functors/hash.hpp"

namespace prototype {

template<class Key, size_t N,
         class Hash = hash<Key>,
         class KeyEqual = void>
class fixed_hash_set {
public:
    struct key_equal_default {
        bool operator()(const Key& a, const Key& b) const { return a == b; }
    };
    typedef typename conditional<is_same<KeyEqual, void>::value,
                                 key_equal_default, KeyEqual>::type equal_type;

    typedef Key    key_type;
    typedef size_t size_type;

    struct entry {
        Key    key;
        size_t dist;
        bool   occupied;
        entry() : key(), dist(0), occupied(false) {}
    };

    fixed_hash_set() : size_(0) {}

    size_type size() const { return size_; }
    size_type capacity() const { return N; }
    bool empty() const { return size_ == 0; }
    bool full() const { return size_ == N; }

    bool insert(const Key& key) {
        PROTOTYPE_ASSERT(!full());
        size_t idx = hash_(key) % N;
        size_t dist = 0;
        Key k = key;

        for (;;) {
            if (!entries_[idx].occupied) {
                entries_[idx].key = k;
                entries_[idx].dist = dist;
                entries_[idx].occupied = true;
                ++size_;
                return true;
            }
            if (equal_(entries_[idx].key, k)) {
                return false; // already present
            }
            // Robin Hood: steal from the rich
            if (entries_[idx].dist < dist) {
                Key tmp_k = entries_[idx].key;
                size_t tmp_d = entries_[idx].dist;
                entries_[idx].key = k;
                entries_[idx].dist = dist;
                k = tmp_k;
                dist = tmp_d;
            }
            idx = (idx + 1) % N;
            ++dist;
        }
    }

    bool contains(const Key& key) const {
        size_t idx = hash_(key) % N;
        size_t dist = 0;
        while (entries_[idx].occupied) {
            if (entries_[idx].dist < dist) return false;
            if (equal_(entries_[idx].key, key)) return true;
            idx = (idx + 1) % N;
            ++dist;
        }
        return false;
    }

    bool erase(const Key& key) {
        size_t idx = hash_(key) % N;
        size_t dist = 0;
        while (entries_[idx].occupied) {
            if (entries_[idx].dist < dist) return false;
            if (equal_(entries_[idx].key, key)) {
                entries_[idx].occupied = false;
                --size_;
                size_t next = (idx + 1) % N;
                while (entries_[next].occupied && entries_[next].dist > 0) {
                    entries_[idx] = entries_[next];
                    entries_[idx].dist--;
                    entries_[next].occupied = false;
                    idx = next;
                    next = (next + 1) % N;
                }
                return true;
            }
            idx = (idx + 1) % N;
            ++dist;
        }
        return false;
    }

    void clear() {
        for (size_t i = 0; i < N; ++i) {
            entries_[i].occupied = false;
            entries_[i].dist = 0;
        }
        size_ = 0;
    }

private:
    entry      entries_[N];
    size_type  size_;
    Hash       hash_;
    equal_type equal_;
};

} // namespace prototype

#endif // PROTOTYPE_ADVANCED_HASH_SET_HPP
