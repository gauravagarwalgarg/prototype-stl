///////////////////////////////////////////////////////////////////////////////
// include/prototype/core/string_view.hpp
//
// PrototypeSTL - Non-owning view over character data
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_CORE_STRING_VIEW_HPP
#define PROTOTYPE_CORE_STRING_VIEW_HPP

#include "../config.hpp"

namespace prototype {

class string_view {
public:
    typedef char         value_type;
    typedef const char*  const_iterator;
    typedef size_t       size_type;
    static const size_type npos = static_cast<size_type>(-1);

    // Constructors
    constexpr string_view() : data_(nullptr), size_(0) {}

    string_view(const char* s) : data_(s), size_(0) {
        if (s) { while (s[size_]) ++size_; }
    }

    constexpr string_view(const char* s, size_type len) : data_(s), size_(len) {}

    // Iterators
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ + size_; }

    // Capacity
    constexpr size_type size() const { return size_; }
    constexpr size_type length() const { return size_; }
    constexpr bool empty() const { return size_ == 0; }

    // Element access
    const char& operator[](size_type pos) const {
        PROTOTYPE_DEBUG_ASSERT(pos < size_);
        return data_[pos];
    }

    const char& at(size_type pos) const {
        PROTOTYPE_ASSERT(pos < size_);
        return data_[pos];
    }

    const char& front() const { return data_[0]; }
    const char& back() const { return data_[size_ - 1]; }
    const char* data() const { return data_; }

    // Modifiers
    void remove_prefix(size_type n) {
        PROTOTYPE_ASSERT(n <= size_);
        data_ += n;
        size_ -= n;
    }

    void remove_suffix(size_type n) {
        PROTOTYPE_ASSERT(n <= size_);
        size_ -= n;
    }

    // Operations
    string_view substr(size_type pos = 0, size_type count = npos) const {
        PROTOTYPE_ASSERT(pos <= size_);
        size_type rlen = (count < size_ - pos) ? count : (size_ - pos);
        return string_view(data_ + pos, rlen);
    }

    int compare(string_view other) const {
        size_type rlen = size_ < other.size_ ? size_ : other.size_;
        for (size_type i = 0; i < rlen; ++i) {
            if (data_[i] < other.data_[i]) return -1;
            if (data_[i] > other.data_[i]) return 1;
        }
        if (size_ < other.size_) return -1;
        if (size_ > other.size_) return 1;
        return 0;
    }

    bool starts_with(string_view prefix) const {
        if (prefix.size_ > size_) return false;
        for (size_type i = 0; i < prefix.size_; ++i) {
            if (data_[i] != prefix.data_[i]) return false;
        }
        return true;
    }

    bool ends_with(string_view suffix) const {
        if (suffix.size_ > size_) return false;
        size_type offset = size_ - suffix.size_;
        for (size_type i = 0; i < suffix.size_; ++i) {
            if (data_[offset + i] != suffix.data_[i]) return false;
        }
        return true;
    }

    size_type find(char c, size_type pos = 0) const {
        for (size_type i = pos; i < size_; ++i) {
            if (data_[i] == c) return i;
        }
        return npos;
    }

    size_type find(string_view sv, size_type pos = 0) const {
        if (sv.size_ == 0) return pos <= size_ ? pos : npos;
        if (sv.size_ > size_) return npos;
        for (size_type i = pos; i <= size_ - sv.size_; ++i) {
            bool match = true;
            for (size_type j = 0; j < sv.size_; ++j) {
                if (data_[i + j] != sv.data_[j]) { match = false; break; }
            }
            if (match) return i;
        }
        return npos;
    }

    size_type rfind(char c, size_type pos = npos) const {
        if (size_ == 0) return npos;
        size_type start = (pos < size_) ? pos : size_ - 1;
        for (size_type i = start + 1; i > 0; --i) {
            if (data_[i - 1] == c) return i - 1;
        }
        return npos;
    }

    size_type rfind(string_view sv, size_type pos = npos) const {
        if (sv.size_ == 0) return (pos < size_) ? pos : size_;
        if (sv.size_ > size_) return npos;
        size_type start = size_ - sv.size_;
        if (pos < start) start = pos;
        for (size_type i = start + 1; i > 0; --i) {
            bool match = true;
            for (size_type j = 0; j < sv.size_; ++j) {
                if (data_[i - 1 + j] != sv.data_[j]) { match = false; break; }
            }
            if (match) return i - 1;
        }
        return npos;
    }

    // Comparison operators
    friend bool operator==(string_view a, string_view b) {
        return a.compare(b) == 0;
    }
    friend bool operator!=(string_view a, string_view b) {
        return a.compare(b) != 0;
    }
    friend bool operator<(string_view a, string_view b) {
        return a.compare(b) < 0;
    }
    friend bool operator<=(string_view a, string_view b) {
        return a.compare(b) <= 0;
    }
    friend bool operator>(string_view a, string_view b) {
        return a.compare(b) > 0;
    }
    friend bool operator>=(string_view a, string_view b) {
        return a.compare(b) >= 0;
    }

private:
    const char* data_;
    size_type   size_;
};

} // namespace prototype

#endif // PROTOTYPE_CORE_STRING_VIEW_HPP
