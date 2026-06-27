///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/flat_map.hpp
//
// PrototypeSTL - Sorted vector-based map with fixed capacity
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_FLAT_MAP_HPP
#define PROTOTYPE_FLAT_MAP_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"

namespace prototype {

template<class Key, class Value, size_t Capacity>
class flat_map {
public:
    typedef pair<Key, Value> value_type;
    typedef value_type*       iterator;
    typedef const value_type* const_iterator;
    typedef size_t            size_type;

    flat_map() : size_(0) {}

    // Iterators
    iterator begin() { return data_; }
    iterator end() { return data_ + size_; }
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ + size_; }

    // Capacity
    bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }
    size_type capacity() const { return Capacity; }

    // Lookup
    iterator find(const Key& key) {
        size_type lo = 0, hi = size_;
        while (lo < hi) {
            size_type mid = lo + (hi - lo) / 2;
            if (data_[mid].first < key) lo = mid + 1;
            else if (key < data_[mid].first) hi = mid;
            else return data_ + mid;
        }
        return end();
    }

    const_iterator find(const Key& key) const {
        size_type lo = 0, hi = size_;
        while (lo < hi) {
            size_type mid = lo + (hi - lo) / 2;
            if (data_[mid].first < key) lo = mid + 1;
            else if (key < data_[mid].first) hi = mid;
            else return data_ + mid;
        }
        return end();
    }

    bool contains(const Key& key) const {
        return find(key) != end();
    }

    // Element access
    Value& operator[](const Key& key) {
        iterator it = find(key);
        if (it != end()) return it->second;
        // Insert with default value
        insert(make_pair(key, Value()));
        return find(key)->second;
    }

    // Modifiers
    bool insert(const value_type& kv) {
        size_type pos = lower_bound_idx(kv.first);
        // Check for duplicate key
        if (pos < size_ && !(kv.first < data_[pos].first) && !(data_[pos].first < kv.first)) {
            return false;
        }
        PROTOTYPE_ASSERT(size_ < Capacity);
        for (size_type i = size_; i > pos; --i) {
            data_[i] = data_[i - 1];
        }
        data_[pos] = kv;
        ++size_;
        return true;
    }

    bool erase(const Key& key) {
        size_type pos = lower_bound_idx(key);
        if (pos >= size_ || key < data_[pos].first || data_[pos].first < key) {
            return false;
        }
        for (size_type i = pos; i + 1 < size_; ++i) {
            data_[i] = data_[i + 1];
        }
        --size_;
        return true;
    }

    void clear() { size_ = 0; }

private:
    size_type lower_bound_idx(const Key& key) const {
        size_type lo = 0, hi = size_;
        while (lo < hi) {
            size_type mid = lo + (hi - lo) / 2;
            if (data_[mid].first < key) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }

    value_type data_[Capacity];
    size_type  size_;
};

} // namespace prototype

#endif // PROTOTYPE_FLAT_MAP_HPP
