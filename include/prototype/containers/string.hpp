///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/string.hpp
//
// PrototypeSTL - fixed_string (stack-allocated string with fixed capacity)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_STRING_HPP
#define PROTOTYPE_STRING_HPP

#include "../config.hpp"

namespace prototype {

template<size_t Capacity>
class fixed_string {
public:
    typedef char        value_type;
    typedef char*       iterator;
    typedef const char* const_iterator;
    typedef size_t      size_type;

    static const size_type npos = static_cast<size_type>(-1);

    fixed_string() : size_(0) { data_[0] = '\0'; }

    fixed_string(const char* s) : size_(0) {
        if (s) {
            while (s[size_] && size_ < Capacity - 1) {
                data_[size_] = s[size_];
                ++size_;
            }
        }
        data_[size_] = '\0';
    }

    fixed_string(const char* s, size_type count) : size_(0) {
        size_type n = count < Capacity - 1 ? count : Capacity - 1;
        for (size_type i = 0; i < n && s[i]; ++i) {
            data_[i] = s[i];
            ++size_;
        }
        data_[size_] = '\0';
    }

    // Element access
    char& operator[](size_type i) { PROTOTYPE_DEBUG_ASSERT(i < size_); return data_[i]; }
    const char& operator[](size_type i) const { PROTOTYPE_DEBUG_ASSERT(i < size_); return data_[i]; }
    char* c_str() { return data_; }
    const char* c_str() const { return data_; }
    char* data() { return data_; }
    const char* data() const { return data_; }

    // Iterators
    iterator begin() { return data_; }
    iterator end() { return data_ + size_; }
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ + size_; }

    // Capacity
    size_type size() const { return size_; }
    size_type length() const { return size_; }
    size_type capacity() const { return Capacity - 1; }
    bool empty() const { return size_ == 0; }

    // Modifiers
    void clear() { size_ = 0; data_[0] = '\0'; }

    void push_back(char c) {
        if (size_ < Capacity - 1) {
            data_[size_++] = c;
            data_[size_] = '\0';
        }
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
            data_[size_] = '\0';
        }
    }

    fixed_string& append(const char* s) {
        while (*s && size_ < Capacity - 1) {
            data_[size_++] = *s++;
        }
        data_[size_] = '\0';
        return *this;
    }

    fixed_string& append(const char* s, size_type count) {
        for (size_type i = 0; i < count && size_ < Capacity - 1; ++i) {
            data_[size_++] = s[i];
        }
        data_[size_] = '\0';
        return *this;
    }

    // Operations
    size_type find(char c, size_type pos = 0) const {
        for (size_type i = pos; i < size_; ++i) {
            if (data_[i] == c) return i;
        }
        return npos;
    }

    size_type find(const char* s, size_type pos = 0) const {
        if (!s) return npos;
        size_type slen = 0;
        while (s[slen]) ++slen;
        if (slen == 0) return pos;
        for (size_type i = pos; i + slen <= size_; ++i) {
            bool match = true;
            for (size_type j = 0; j < slen; ++j) {
                if (data_[i + j] != s[j]) { match = false; break; }
            }
            if (match) return i;
        }
        return npos;
    }

    fixed_string substr(size_type pos, size_type count = npos) const {
        if (pos >= size_) return fixed_string();
        size_type n = (count == npos || pos + count > size_) ? size_ - pos : count;
        return fixed_string(data_ + pos, n);
    }

    int compare(const fixed_string& other) const {
        size_type n = size_ < other.size_ ? size_ : other.size_;
        for (size_type i = 0; i < n; ++i) {
            if (data_[i] < other.data_[i]) return -1;
            if (data_[i] > other.data_[i]) return 1;
        }
        if (size_ < other.size_) return -1;
        if (size_ > other.size_) return 1;
        return 0;
    }

    bool operator==(const fixed_string& o) const { return compare(o) == 0; }
    bool operator!=(const fixed_string& o) const { return compare(o) != 0; }
    bool operator<(const fixed_string& o) const { return compare(o) < 0; }

private:
    char    data_[Capacity];
    size_type size_;
};

} // namespace prototype

#endif // PROTOTYPE_STRING_HPP
