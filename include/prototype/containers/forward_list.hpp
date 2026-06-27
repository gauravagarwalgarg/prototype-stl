///////////////////////////////////////////////////////////////////////////////
// include/prototype/containers/forward_list.hpp
//
// PrototypeSTL - Singly-linked list with fixed capacity
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_FORWARD_LIST_HPP
#define PROTOTYPE_FORWARD_LIST_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"

namespace prototype {

template<class T, size_t Capacity>
class forward_list {
    struct node {
        T   value;
        int next;
    };

public:
    class iterator {
    public:
        typedef T         value_type;
        typedef T&        reference;
        typedef T*        pointer;
        typedef ptrdiff_t difference_type;

        iterator() : list_(nullptr), idx_(-1) {}
        iterator(forward_list* l, int i) : list_(l), idx_(i) {}

        reference operator*() const { return list_->nodes_[idx_].value; }
        pointer operator->() const { return &list_->nodes_[idx_].value; }
        iterator& operator++() { idx_ = list_->nodes_[idx_].next; return *this; }
        iterator operator++(int) { iterator t = *this; ++(*this); return t; }
        bool operator==(const iterator& o) const { return idx_ == o.idx_; }
        bool operator!=(const iterator& o) const { return idx_ != o.idx_; }

        int index() const { return idx_; }
    private:
        forward_list* list_;
        int idx_;
    };

    forward_list() : size_(0), head_(-1), free_(-1) {
        for (size_t i = 0; i < Capacity; ++i) {
            nodes_[i].next = (int)(i + 1 < Capacity ? i + 1 : -1);
        }
        free_ = 0;
    }

    ~forward_list() {}

    // Element access
    T& front() { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return nodes_[head_].value; }
    const T& front() const { PROTOTYPE_DEBUG_ASSERT(size_ > 0); return nodes_[head_].value; }

    // Iterators
    iterator begin() { return iterator(this, head_); }
    iterator end() { return iterator(this, -1); }

    // Capacity
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }

    // Modifiers
    void push_front(const T& value) {
        PROTOTYPE_ASSERT(size_ < Capacity);
        int idx = alloc_node();
        nodes_[idx].value = value;
        nodes_[idx].next = head_;
        head_ = idx;
        ++size_;
    }

    void pop_front() {
        PROTOTYPE_DEBUG_ASSERT(size_ > 0);
        int idx = head_;
        head_ = nodes_[idx].next;
        free_node(idx);
        --size_;
    }

    void clear() {
        while (size_ > 0) pop_front();
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
        free_ = idx;
    }

    node   nodes_[Capacity];
    size_t size_;
    int    head_;
    int    free_;
};

} // namespace prototype

#endif // PROTOTYPE_FORWARD_LIST_HPP
