///////////////////////////////////////////////////////////////////////////////
// include/prototype/advanced/priority_queue.hpp
//
// PrototypeSTL - Fixed-capacity binary heap priority queue
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ADVANCED_PRIORITY_QUEUE_HPP
#define PROTOTYPE_ADVANCED_PRIORITY_QUEUE_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"

namespace prototype {

template<class T, size_t N, class Compare = void>
class fixed_priority_queue {
public:
    struct compare_default {
        bool operator()(const T& a, const T& b) const { return a < b; }
    };
    typedef typename conditional<is_same<Compare, void>::value,
                                 compare_default, Compare>::type compare_type;
    typedef T         value_type;
    typedef size_t    size_type;

    fixed_priority_queue() : size_(0) {}

    void push(const T& value) {
        PROTOTYPE_ASSERT(size_ < N);
        data_[size_] = value;
        sift_up(size_);
        ++size_;
    }

    void push(T&& value) {
        PROTOTYPE_ASSERT(size_ < N);
        data_[size_] = prototype::move(value);
        sift_up(size_);
        ++size_;
    }

    void pop() {
        PROTOTYPE_ASSERT(size_ > 0);
        data_[0] = prototype::move(data_[size_ - 1]);
        --size_;
        if (size_ > 0) sift_down(0);
    }

    const T& top() const {
        PROTOTYPE_ASSERT(size_ > 0);
        return data_[0];
    }

    bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }
    bool full() const { return size_ == N; }
    constexpr size_type capacity() const { return N; }

private:
    void sift_up(size_t idx) {
        while (idx > 0) {
            size_t parent = (idx - 1) / 2;
            if (comp_(data_[parent], data_[idx])) {
                prototype::swap(data_[parent], data_[idx]);
                idx = parent;
            } else {
                break;
            }
        }
    }

    void sift_down(size_t idx) {
        while (true) {
            size_t largest = idx;
            size_t left = 2 * idx + 1;
            size_t right = 2 * idx + 2;

            if (left < size_ && comp_(data_[largest], data_[left])) {
                largest = left;
            }
            if (right < size_ && comp_(data_[largest], data_[right])) {
                largest = right;
            }
            if (largest != idx) {
                prototype::swap(data_[idx], data_[largest]);
                idx = largest;
            } else {
                break;
            }
        }
    }

    T            data_[N];
    size_type    size_;
    compare_type comp_;
};

} // namespace prototype

#endif // PROTOTYPE_ADVANCED_PRIORITY_QUEUE_HPP
