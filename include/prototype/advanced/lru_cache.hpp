///////////////////////////////////////////////////////////////////////////////
// include/prototype/advanced/lru_cache.hpp
//
// PrototypeSTL - Fixed-capacity Least-Recently-Used cache
// Implemented with hash map + doubly-linked list (all from fixed storage).
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ADVANCED_LRU_CACHE_HPP
#define PROTOTYPE_ADVANCED_LRU_CACHE_HPP

#include "../config.hpp"
#include "../containers/optional.hpp"
#include "../functors/hash.hpp"

namespace prototype {

template<class Key, class Value, size_t N, class Hash = hash<Key>>
class fixed_lru_cache {
public:
    typedef Key    key_type;
    typedef Value  mapped_type;
    typedef size_t size_type;

    fixed_lru_cache() : head_(INVALID), tail_(INVALID), size_(0) {
        for (size_t i = 0; i < TABLE_SIZE; ++i) {
            buckets_[i] = INVALID;
        }
        for (size_t i = 0; i < N; ++i) {
            entries_[i].occupied = false;
            entries_[i].prev = INVALID;
            entries_[i].next = INVALID;
            entries_[i].bucket_next = INVALID;
        }
    }

    /// Get value for key, marks it as most recently used
    optional<Value> get(const Key& key) {
        size_t idx = find_entry(key);
        if (idx == INVALID) return optional<Value>();
        move_to_front(idx);
        return optional<Value>(entries_[idx].value);
    }

    /// Insert or update key-value pair. Evicts LRU entry if full.
    void put(const Key& key, const Value& value) {
        size_t idx = find_entry(key);
        if (idx != INVALID) {
            entries_[idx].value = value;
            move_to_front(idx);
            return;
        }

        if (size_ == N) {
            // Evict LRU (tail)
            evict_tail();
        }

        // Find a free slot
        size_t slot = find_free_slot();
        PROTOTYPE_ASSERT(slot != INVALID);

        entries_[slot].key = key;
        entries_[slot].value = value;
        entries_[slot].occupied = true;

        // Add to hash table
        size_t bucket = hash_(key) % TABLE_SIZE;
        entries_[slot].bucket_next = buckets_[bucket];
        buckets_[bucket] = slot;

        // Add to front of LRU list
        entries_[slot].prev = INVALID;
        entries_[slot].next = head_;
        if (head_ != INVALID) entries_[head_].prev = slot;
        head_ = slot;
        if (tail_ == INVALID) tail_ = slot;

        ++size_;
    }

    bool contains(const Key& key) const {
        return find_entry(key) != INVALID;
    }

    size_type size() const { return size_; }
    constexpr size_type capacity() const { return N; }
    bool empty() const { return size_ == 0; }
    bool full() const { return size_ == N; }

    void clear() {
        for (size_t i = 0; i < TABLE_SIZE; ++i) buckets_[i] = INVALID;
        for (size_t i = 0; i < N; ++i) {
            entries_[i].occupied = false;
            entries_[i].prev = INVALID;
            entries_[i].next = INVALID;
            entries_[i].bucket_next = INVALID;
        }
        head_ = INVALID;
        tail_ = INVALID;
        size_ = 0;
    }

private:
    static const size_t INVALID = static_cast<size_t>(-1);
    // Use a larger table size than capacity for fewer collisions
    static const size_t TABLE_SIZE = N * 2 > 1 ? N * 2 : 1;

    struct entry {
        Key    key;
        Value  value;
        bool   occupied;
        size_t prev;        // LRU list prev
        size_t next;        // LRU list next
        size_t bucket_next; // hash chain next
    };

    size_t find_entry(const Key& key) const {
        size_t bucket = hash_(key) % TABLE_SIZE;
        size_t idx = buckets_[bucket];
        while (idx != INVALID) {
            if (entries_[idx].occupied && entries_[idx].key == key) {
                return idx;
            }
            idx = entries_[idx].bucket_next;
        }
        return INVALID;
    }

    size_t find_free_slot() const {
        for (size_t i = 0; i < N; ++i) {
            if (!entries_[i].occupied) return i;
        }
        return INVALID;
    }

    void move_to_front(size_t idx) {
        if (idx == head_) return;

        // Remove from current position
        if (entries_[idx].prev != INVALID)
            entries_[entries_[idx].prev].next = entries_[idx].next;
        if (entries_[idx].next != INVALID)
            entries_[entries_[idx].next].prev = entries_[idx].prev;
        if (idx == tail_)
            tail_ = entries_[idx].prev;

        // Place at front
        entries_[idx].prev = INVALID;
        entries_[idx].next = head_;
        if (head_ != INVALID) entries_[head_].prev = idx;
        head_ = idx;
    }

    void evict_tail() {
        PROTOTYPE_ASSERT(tail_ != INVALID);
        size_t idx = tail_;

        // Remove from LRU list
        if (entries_[idx].prev != INVALID) {
            entries_[entries_[idx].prev].next = INVALID;
        }
        tail_ = entries_[idx].prev;
        if (head_ == idx) head_ = INVALID;

        // Remove from hash table
        size_t bucket = hash_(entries_[idx].key) % TABLE_SIZE;
        size_t* p = &buckets_[bucket];
        while (*p != INVALID) {
            if (*p == idx) {
                *p = entries_[idx].bucket_next;
                break;
            }
            p = &entries_[*p].bucket_next;
        }

        entries_[idx].occupied = false;
        entries_[idx].prev = INVALID;
        entries_[idx].next = INVALID;
        entries_[idx].bucket_next = INVALID;
        --size_;
    }

    entry     entries_[N];
    size_t    buckets_[TABLE_SIZE];
    size_t    head_;
    size_t    tail_;
    size_type size_;
    Hash      hash_;
};

} // namespace prototype

#endif // PROTOTYPE_ADVANCED_LRU_CACHE_HPP
