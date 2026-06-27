///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/vector.hpp
//
// PrototypeSTL - Dynamic vector and fixed_vector
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_VECTOR_HPP
#define PROTOTYPE_VECTOR_HPP

#include "../config.hpp"
#include "../type_traits/type_traits.hpp"
#include "../utility/utility.hpp"
#include "../iterators/iterator_traits.hpp"
#include "../allocators/allocator.hpp"

namespace prototype {

///////////////////////////////////////////////////////////////////////////////
// vector<T> - heap-allocated dynamic array
///////////////////////////////////////////////////////////////////////////////
template<class T, class Allocator = allocator<T> >
class vector {
public:
    typedef T              value_type;
    typedef T*             iterator;
    typedef const T*       const_iterator;
    typedef T&             reference;
    typedef const T&       const_reference;
    typedef size_t         size_type;
    typedef ptrdiff_t      difference_type;

    vector() : data_(nullptr), size_(0), capacity_(0) {}

    explicit vector(size_type count) : data_(nullptr), size_(0), capacity_(0) {
        resize(count);
    }

    vector(size_type count, const T& value) : data_(nullptr), size_(0), capacity_(0) {
        reserve(count);
        for (size_type i = 0; i < count; ++i) push_back(value);
    }

    vector(const vector& other) : data_(nullptr), size_(0), capacity_(0) {
        reserve(other.size_);
        for (size_type i = 0; i < other.size_; ++i) {
            ::new(static_cast<void*>(data_ + i)) T(other.data_[i]);
        }
        size_ = other.size_;
    }

    vector(vector&& other) : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~vector() { destroy_all(); dealloc(); }

    vector& operator=(const vector& other) {
        if (this != &other) {
            clear();
            reserve(other.size_);
            for (size_type i = 0; i < other.size_; ++i) {
                ::new(static_cast<void*>(data_ + i)) T(other.data_[i]);
            }
            size_ = other.size_;
        }
        return *this;
    }

    vector& operator=(vector&& other) {
        if (this != &other) {
            destroy_all(); dealloc();
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    // Element access
    reference operator[](size_type i) { PROTOTYPE_DEBUG_ASSERT(i < size_); return data_[i]; }
    const_reference operator[](size_type i) const { PROTOTYPE_DEBUG_ASSERT(i < size_); return data_[i]; }
    reference at(size_type i) { PROTOTYPE_ASSERT(i < size_); return data_[i]; }
    const_reference at(size_type i) const { PROTOTYPE_ASSERT(i < size_); return data_[i]; }
    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }
    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }
    T* data() { return data_; }
    const T* data() const { return data_; }

    // Iterators
    iterator begin() { return data_; }
    iterator end() { return data_ + size_; }
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ + size_; }

    // Capacity
    bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }
    size_type capacity() const { return capacity_; }

    void reserve(size_type new_cap) {
        if (new_cap <= capacity_) return;
        T* new_data = alloc_.allocate(new_cap);
        for (size_type i = 0; i < size_; ++i) {
            ::new(static_cast<void*>(new_data + i)) T(prototype::move(data_[i]));
            data_[i].~T();
        }
        dealloc();
        data_ = new_data;
        capacity_ = new_cap;
    }

    void resize(size_type count) {
        if (count < size_) {
            for (size_type i = count; i < size_; ++i) data_[i].~T();
            size_ = count;
        } else if (count > size_) {
            reserve(count);
            for (size_type i = size_; i < count; ++i) {
                ::new(static_cast<void*>(data_ + i)) T();
            }
            size_ = count;
        }
    }

    // Modifiers
    void push_back(const T& value) {
        if (size_ == capacity_) grow();
        ::new(static_cast<void*>(data_ + size_)) T(value);
        ++size_;
    }

    void push_back(T&& value) {
        if (size_ == capacity_) grow();
        ::new(static_cast<void*>(data_ + size_)) T(prototype::move(value));
        ++size_;
    }

    template<class... Args>
    void emplace_back(Args&&... args) {
        if (size_ == capacity_) grow();
        ::new(static_cast<void*>(data_ + size_)) T(prototype::forward<Args>(args)...);
        ++size_;
    }

    void pop_back() {
        PROTOTYPE_DEBUG_ASSERT(size_ > 0);
        --size_;
        data_[size_].~T();
    }

    void clear() { destroy_all(); size_ = 0; }

    iterator erase(iterator pos) {
        pos->~T();
        for (iterator it = pos; it + 1 != end(); ++it) {
            ::new(static_cast<void*>(it)) T(prototype::move(*(it + 1)));
            (it + 1)->~T();
        }
        --size_;
        return pos;
    }

    iterator insert(iterator pos, const T& value) {
        size_type idx = static_cast<size_type>(pos - data_);
        if (size_ == capacity_) {
            grow();
            pos = data_ + idx;
        }
        // Move elements to the right
        if (size_ > 0) {
            ::new(static_cast<void*>(data_ + size_)) T(prototype::move(data_[size_ - 1]));
            for (size_type i = size_ - 1; i > idx; --i) {
                data_[i] = prototype::move(data_[i - 1]);
            }
            data_[idx] = value;
        } else {
            ::new(static_cast<void*>(data_)) T(value);
        }
        ++size_;
        return data_ + idx;
    }

    void swap(vector& other) {
        prototype::swap(data_, other.data_);
        prototype::swap(size_, other.size_);
        prototype::swap(capacity_, other.capacity_);
    }

private:
    void grow() {
        size_type new_cap = capacity_ == 0 ? 4 : capacity_ * 2;
        reserve(new_cap);
    }

    void destroy_all() {
        for (size_type i = 0; i < size_; ++i) data_[i].~T();
    }

    void dealloc() {
        if (data_) { alloc_.deallocate(data_, capacity_); data_ = nullptr; }
    }

    T*         data_;
    size_type  size_;
    size_type  capacity_;
    Allocator  alloc_;
};

///////////////////////////////////////////////////////////////////////////////
// fixed_vector<T, N> - stack-allocated vector with fixed capacity
///////////////////////////////////////////////////////////////////////////////
template<class T, size_t Capacity>
class fixed_vector {
public:
    typedef T              value_type;
    typedef T*             iterator;
    typedef const T*       const_iterator;
    typedef T&             reference;
    typedef const T&       const_reference;
    typedef size_t         size_type;

    fixed_vector() : size_(0) {}

    reference operator[](size_type i) { PROTOTYPE_DEBUG_ASSERT(i < size_); return *ptr(i); }
    const_reference operator[](size_type i) const { PROTOTYPE_DEBUG_ASSERT(i < size_); return *ptr(i); }
    reference front() { return *ptr(0); }
    const_reference front() const { return *ptr(0); }
    reference back() { return *ptr(size_ - 1); }
    const_reference back() const { return *ptr(size_ - 1); }
    T* data() { return ptr(0); }
    const T* data() const { return ptr(0); }

    iterator begin() { return ptr(0); }
    iterator end() { return ptr(size_); }
    const_iterator begin() const { return ptr(0); }
    const_iterator end() const { return ptr(size_); }

    bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }
    constexpr size_type capacity() const { return Capacity; }
    bool full() const { return size_ == Capacity; }

    void push_back(const T& value) {
        PROTOTYPE_ASSERT(size_ < Capacity);
        ::new(static_cast<void*>(ptr(size_))) T(value);
        ++size_;
    }

    void push_back(T&& value) {
        PROTOTYPE_ASSERT(size_ < Capacity);
        ::new(static_cast<void*>(ptr(size_))) T(prototype::move(value));
        ++size_;
    }

    void pop_back() {
        PROTOTYPE_DEBUG_ASSERT(size_ > 0);
        --size_;
        ptr(size_)->~T();
    }

    void clear() {
        for (size_type i = 0; i < size_; ++i) ptr(i)->~T();
        size_ = 0;
    }

    ~fixed_vector() { clear(); }

private:
    T* ptr(size_type i) { return reinterpret_cast<T*>(storage_) + i; }
    const T* ptr(size_type i) const { return reinterpret_cast<const T*>(storage_) + i; }

    PROTOTYPE_ALIGNAS(alignof(T)) char storage_[sizeof(T) * Capacity];
    size_type size_;
};

} // namespace prototype

#endif // PROTOTYPE_VECTOR_HPP
