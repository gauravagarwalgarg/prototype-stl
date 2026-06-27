///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/static_map.hpp
//
// PrototypeSTL - Fixed-capacity hash map (open addressing, linear probing)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_STATIC_MAP_HPP
#define PROTOTYPE_STATIC_MAP_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"

namespace prototype {

template<class Key, class Value, size_t Capacity>
class static_map {
    enum slot_state { EMPTY = 0, OCCUPIED = 1, DELETED = 2 };

    struct slot {
        pair<Key, Value> kv;
        slot_state       state;
    };

public:
    static_map() : size_(0) {
        for (size_t i = 0; i < Capacity; ++i) {
            slots_[i].state = EMPTY;
        }
    }

    typedef pair<Key, Value>* iterator;
    typedef const pair<Key, Value>* const_iterator;

    // Capacity
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    size_t capacity() const { return Capacity; }

    // Lookup
    Value* find(const Key& key) {
        size_t idx = probe(key);
        if (idx == Capacity) return nullptr;
        return &slots_[idx].kv.second;
    }

    const Value* find(const Key& key) const {
        size_t idx = probe(key);
        if (idx == Capacity) return nullptr;
        return &slots_[idx].kv.second;
    }

    bool contains(const Key& key) const {
        return probe(key) != Capacity;
    }

    // Modifiers
    bool insert(const Key& key, const Value& value) {
        if (size_ >= Capacity * 3 / 4) return false; // load factor limit
        size_t h = hash(key);
        for (size_t i = 0; i < Capacity; ++i) {
            size_t idx = (h + i) % Capacity;
            if (slots_[idx].state == EMPTY || slots_[idx].state == DELETED) {
                slots_[idx].kv.first = key;
                slots_[idx].kv.second = value;
                slots_[idx].state = OCCUPIED;
                ++size_;
                return true;
            }
            if (slots_[idx].state == OCCUPIED && slots_[idx].kv.first == key) {
                return false; // duplicate
            }
        }
        return false;
    }

    bool erase(const Key& key) {
        size_t idx = probe(key);
        if (idx == Capacity) return false;
        slots_[idx].state = DELETED;
        --size_;
        return true;
    }

    Value& operator[](const Key& key) {
        size_t h = hash(key);
        for (size_t i = 0; i < Capacity; ++i) {
            size_t idx = (h + i) % Capacity;
            if (slots_[idx].state == OCCUPIED && slots_[idx].kv.first == key) {
                return slots_[idx].kv.second;
            }
            if (slots_[idx].state == EMPTY) {
                slots_[idx].kv.first = key;
                slots_[idx].kv.second = Value();
                slots_[idx].state = OCCUPIED;
                ++size_;
                return slots_[idx].kv.second;
            }
        }
        PROTOTYPE_PANIC("static_map full");
        return slots_[0].kv.second; // unreachable
    }

    void clear() {
        for (size_t i = 0; i < Capacity; ++i) slots_[i].state = EMPTY;
        size_ = 0;
    }

private:
    size_t probe(const Key& key) const {
        size_t h = hash(key);
        for (size_t i = 0; i < Capacity; ++i) {
            size_t idx = (h + i) % Capacity;
            if (slots_[idx].state == EMPTY) return Capacity;
            if (slots_[idx].state == OCCUPIED && slots_[idx].kv.first == key) return idx;
        }
        return Capacity;
    }

    static size_t hash(const Key& key) {
        // FNV-1a inspired hash for integral types
        size_t h = 14695981039346656037ULL;
        const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&key);
        for (size_t i = 0; i < sizeof(Key); ++i) {
            h ^= bytes[i];
            h *= 1099511628211ULL;
        }
        return h;
    }

    slot   slots_[Capacity];
    size_t size_;
};

} // namespace prototype

#endif // PROTOTYPE_STATIC_MAP_HPP
