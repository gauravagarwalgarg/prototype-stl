///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/list.hpp
//
// PrototypeSTL - Doubly-linked list (fixed_list with embedded storage)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_LIST_HPP
#define PROTOTYPE_LIST_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"

namespace prototype {

///////////////////////////////////////////////////////////////////////////////
// fixed_list<T, N> - intrusive doubly-linked list with fixed capacity
///////////////////////////////////////////////////////////////////////////////
template<class T, size_t Capacity>
class fixed_list {
    struct node {
        T     value;
        int   prev;
        int   next;
    };

public:
    class iterator {
    public:
        typedef T         value_type;
        typedef T&        reference;
        typedef T*        pointer;
        typedef ptrdiff_t difference_type;

        iterator() : list_(nullptr), idx_(-1) {}
        iterator(fixed_list* l, int i) : list_(l), idx_(i) {}

        reference operator*() const { return list_->nodes_[idx_].value; }
        pointer operator->() const { return &list_->nodes_[idx_].value; }
        iterator& operator++() { idx_ = list_->nodes_[idx_].next; return *this; }
        iterator operator++(int) { iterator t = *this; ++(*this); return t; }
        iterator& operator--() { idx_ = list_->nodes_[idx_].prev; return *this; }
        bool operator==(const iterator& o) const { return idx_ == o.idx_; }
        bool operator!=(const iterator& o) const { return idx_ != o.idx_; }

        int index() const { return idx_; }
    private:
        fixed_list* list_;
        int idx_;
    };

    fixed_list() : size_(0), head_(-1), tail_(-1), free_(-1) {
        // Initialize free list
        for (size_t i = 0; i < Capacity; ++i) {
            nodes_[i].next = (int)(i + 1 < Capacity ? i + 1 : -1);
            nodes_[i].prev = -1;
        }
        free_ = 0;
    }

    ~fixed_list() {}

    // Element access
    T& front() { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return nodes_[head_].value; }
    const T& front() const { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return nodes_[head_].value; }
    T& back() { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return nodes_[tail_].value; }
    const T& back() const { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return nodes_[tail_].value; }

    // Iterators
    iterator begin() { return iterator(this, head_); }
    iterator end() { return iterator(this, -1); }

    // Capacity
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    size_t capacity() const { return Capacity; }

    // Modifiers
    void push_back(const T& value) {
        PROTOTYPE_ASSERT(size_ < Capacity);
        int idx = alloc_node();
        nodes_[idx].value = value;
        nodes_[idx].prev = tail_;
        nodes_[idx].next = -1;
        if (tail_ >= 0) nodes_[tail_].next = idx;
        tail_ = idx;
        if (head_ < 0) head_ = idx;
        ++size_;
    }

    void push_front(const T& value) {
        PROTOTYPE_ASSERT(size_ < Capacity);
        int idx = alloc_node();
        nodes_[idx].value = value;
        nodes_[idx].prev = -1;
        nodes_[idx].next = head_;
        if (head_ >= 0) nodes_[head_].prev = idx;
        head_ = idx;
        if (tail_ < 0) tail_ = idx;
        ++size_;
    }

    void pop_back() {
        PROTOTYPE_DEBUG_ASSERT(size_ > 0);
        int idx = tail_;
        tail_ = nodes_[idx].prev;
        if (tail_ >= 0) nodes_[tail_].next = -1;
        else head_ = -1;
        free_node(idx);
        --size_;
    }

    void pop_front() {
        PROTOTYPE_DEBUG_ASSERT(size_ > 0);
        int idx = head_;
        head_ = nodes_[idx].next;
        if (head_ >= 0) nodes_[head_].prev = -1;
        else tail_ = -1;
        free_node(idx);
        --size_;
    }

    void clear() {
        while (size_ > 0) pop_back();
    }

    void erase(iterator pos) {
        int idx = pos.index();
        if (idx < 0) return;
        int p = nodes_[idx].prev;
        int n = nodes_[idx].next;
        if (p >= 0) nodes_[p].next = n; else head_ = n;
        if (n >= 0) nodes_[n].prev = p; else tail_ = p;
        free_node(idx);
        --size_;
    }

private:
    int alloc_node() {
        PROTOTYPE_ASSERT(free_ >= 0);
        int idx = free_;
        free_ = nodes_[free_].next;
        return idx;
    }

    void free_node(int idx) {
        nodes_[idx].next = free_;
        nodes_[idx].prev = -1;
        free_ = idx;
    }

    node   nodes_[Capacity];
    size_t size_;
    int    head_;
    int    tail_;
    int    free_;
};

} // namespace prototype

#endif // PROTOTYPE_LIST_HPP
