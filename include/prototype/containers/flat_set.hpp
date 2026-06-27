///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/flat_set.hpp
//
// PrototypeSTL - Sorted vector-based set with fixed capacity
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_FLAT_SET_HPP
#define PROTOTYPE_FLAT_SET_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"

namespace prototype {

template<class T, size_t Capacity>
class flat_set {
public:
    typedef T              value_type;
    typedef const T*       iterator;
    typedef const T*       const_iterator;
    typedef size_t         size_type;

    flat_set() : size_(0) {}

    // Iterators
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ + size_; }

    // Capacity
    bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }
    size_type capacity() const { return Capacity; }

    // Lookup
    const_iterator find(const T& value) const {
        size_type lo = 0, hi = size_;
        while (lo < hi) {
            size_type mid = lo + (hi - lo) / 2;
            if (data_[mid] < value) lo = mid + 1;
            else if (value < data_[mid]) hi = mid;
            else return data_ + mid;
        }
        return end();
    }

    bool contains(const T& value) const {
        return find(value) != end();
    }

    size_type count(const T& value) const {
        return contains(value) ? 1 : 0;
    }

    // Modifiers
    bool insert(const T& value) {
        // Find insertion point
        size_type pos = lower_bound_idx(value);
        // Check for duplicate
        if (pos < size_ && !(value < data_[pos]) && !(data_[pos] < value)) {
            return false; // already exists
        }
        PROTOTYPE_ASSERT(size_ < Capacity);
        // Shift elements right
        for (size_type i = size_; i > pos; --i) {
            data_[i] = data_[i - 1];
        }
        data_[pos] = value;
        ++size_;
        return true;
    }

    bool erase(const T& value) {
        size_type pos = lower_bound_idx(value);
        if (pos >= size_ || value < data_[pos] || data_[pos] < value) {
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
    size_type lower_bound_idx(const T& value) const {
        size_type lo = 0, hi = size_;
        while (lo < hi) {
            size_type mid = lo + (hi - lo) / 2;
            if (data_[mid] < value) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }

    T         data_[Capacity];
    size_type size_;
};

} // namespace prototype

#endif // PROTOTYPE_FLAT_SET_HPP
