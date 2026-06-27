///////////////////////////////////////////////////////////////////////////////
// include/prototype/advanced/hash_map.hpp
//
// PrototypeSTL - Fixed-capacity open-addressing hash map with Robin Hood hashing
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ADVANCED_HASH_MAP_HPP
#define PROTOTYPE_ADVANCED_HASH_MAP_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"
#include "../functors/hash.hpp"
#include "../containers/optional.hpp"

namespace prototype {

template<class Key, class Value, size_t N,
         class Hash = hash<Key>,
         class KeyEqual = void>
class fixed_hash_map {
public:
    struct key_equal_default {
        bool operator()(const Key& a, const Key& b) const { return a == b; }
    };
    typedef typename conditional<is_same<KeyEqual, void>::value,
                                 key_equal_default, KeyEqual>::type equal_type;

    typedef Key    key_type;
    typedef Value  mapped_type;
    typedef pair<Key, Value> value_type;
    typedef size_t size_type;

    struct entry {
        value_type kv;
        size_t     dist;  // probe distance from ideal slot
        bool       occupied;
        entry() : kv(), dist(0), occupied(false) {}
    };

    fixed_hash_map() : size_(0) {}

    size_type size() const { return size_; }
    size_type capacity() const { return N; }
    bool empty() const { return size_ == 0; }
    bool full() const { return size_ == N; }

    bool insert(const Key& key, const Value& value) {
        PROTOTYPE_ASSERT(!full());
        size_t idx = hash_(key) % N;
        size_t dist = 0;
        Key k = key;
        Value v = value;

        for (;;) {
            if (!entries_[idx].occupied) {
                entries_[idx].kv.first = k;
                entries_[idx].kv.second = v;
                entries_[idx].dist = dist;
                entries_[idx].occupied = true;
                ++size_;
                return true;
            }
            // Duplicate key check
            if (equal_(entries_[idx].kv.first, k)) {
                entries_[idx].kv.second = v;
                return false; // updated, not inserted
            }
            // Robin Hood: steal from the rich
            if (entries_[idx].dist < dist) {
                // Swap
                Key tmp_k = entries_[idx].kv.first;
                Value tmp_v = entries_[idx].kv.second;
                size_t tmp_d = entries_[idx].dist;

                entries_[idx].kv.first = k;
                entries_[idx].kv.second = v;
                entries_[idx].dist = dist;

                k = tmp_k;
                v = tmp_v;
                dist = tmp_d;
            }
            idx = (idx + 1) % N;
            ++dist;
        }
    }

    Value* find(const Key& key) {
        size_t idx = hash_(key) % N;
        size_t dist = 0;
        while (entries_[idx].occupied) {
            if (entries_[idx].dist < dist) return nullptr;
            if (equal_(entries_[idx].kv.first, key)) {
                return &entries_[idx].kv.second;
            }
            idx = (idx + 1) % N;
            ++dist;
        }
        return nullptr;
    }

    const Value* find(const Key& key) const {
        size_t idx = hash_(key) % N;
        size_t dist = 0;
        while (entries_[idx].occupied) {
            if (entries_[idx].dist < dist) return nullptr;
            if (equal_(entries_[idx].kv.first, key)) {
                return &entries_[idx].kv.second;
            }
            idx = (idx + 1) % N;
            ++dist;
        }
        return nullptr;
    }

    bool contains(const Key& key) const {
        return find(key) != nullptr;
    }

    bool erase(const Key& key) {
        size_t idx = hash_(key) % N;
        size_t dist = 0;
        while (entries_[idx].occupied) {
            if (entries_[idx].dist < dist) return false;
            if (equal_(entries_[idx].kv.first, key)) {
                // Found, now backward-shift delete
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

    Value& operator[](const Key& key) {
        Value* v = find(key);
        if (v) return *v;
        PROTOTYPE_ASSERT(!full());
        insert(key, Value());
        return *find(key);
    }

private:
    entry      entries_[N];
    size_type  size_;
    Hash       hash_;
    equal_type equal_;
};

} // namespace prototype

#endif // PROTOTYPE_ADVANCED_HASH_MAP_HPP
