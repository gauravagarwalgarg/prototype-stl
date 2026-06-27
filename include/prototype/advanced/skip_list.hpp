///////////////////////////////////////////////////////////////////////////////
// include/prototype/advanced/skip_list.hpp
//
// PrototypeSTL - Probabilistic sorted list with O(log n) search
// Uses deterministic LFSR for reproducible level generation.
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ADVANCED_SKIP_LIST_HPP
#define PROTOTYPE_ADVANCED_SKIP_LIST_HPP

#include "../config.hpp"
#include "../containers/optional.hpp"

namespace prototype {

template<class T, size_t N, size_t MaxLevel = 8, class Compare = void>
class fixed_skip_list {
public:
    struct compare_default {
        bool operator()(const T& a, const T& b) const { return a < b; }
    };
    typedef typename conditional<is_same<Compare, void>::value,
                                 compare_default, Compare>::type compare_type;
    typedef T         value_type;
    typedef size_t    size_type;

    fixed_skip_list() : size_(0), level_(1), lfsr_(0xACE1u) {
        // Head node is nodes_[0], always allocated
        head_ = 0;
        node_count_ = 1;
        nodes_[head_].value = T();
        for (size_t i = 0; i < MaxLevel; ++i) {
            nodes_[head_].forward[i] = INVALID;
        }
    }

    bool insert(const T& value) {
        PROTOTYPE_ASSERT(node_count_ < N + 1); // +1 for head

        size_t update[MaxLevel];
        size_t cur = head_;

        for (int i = static_cast<int>(level_) - 1; i >= 0; --i) {
            while (nodes_[cur].forward[i] != INVALID &&
                   comp_(nodes_[nodes_[cur].forward[i]].value, value)) {
                cur = nodes_[cur].forward[i];
            }
            update[i] = cur;
        }

        // Check for duplicate
        size_t next = nodes_[cur].forward[0];
        if (next != INVALID && !comp_(value, nodes_[next].value) &&
            !comp_(nodes_[next].value, value)) {
            return false; // duplicate
        }

        size_t new_level = random_level();
        if (new_level > level_) {
            for (size_t i = level_; i < new_level; ++i) {
                update[i] = head_;
            }
            level_ = new_level;
        }

        size_t new_node = node_count_++;
        nodes_[new_node].value = value;
        for (size_t i = 0; i < MaxLevel; ++i) {
            nodes_[new_node].forward[i] = INVALID;
        }

        for (size_t i = 0; i < new_level; ++i) {
            nodes_[new_node].forward[i] = nodes_[update[i]].forward[i];
            nodes_[update[i]].forward[i] = new_node;
        }

        ++size_;
        return true;
    }

    bool find(const T& value) const {
        size_t cur = head_;
        for (int i = static_cast<int>(level_) - 1; i >= 0; --i) {
            while (nodes_[cur].forward[i] != INVALID &&
                   comp_(nodes_[nodes_[cur].forward[i]].value, value)) {
                cur = nodes_[cur].forward[i];
            }
        }
        size_t next = nodes_[cur].forward[0];
        return next != INVALID && !comp_(value, nodes_[next].value) &&
               !comp_(nodes_[next].value, value);
    }

    bool erase(const T& value) {
        size_t update[MaxLevel];
        size_t cur = head_;

        for (int i = static_cast<int>(level_) - 1; i >= 0; --i) {
            while (nodes_[cur].forward[i] != INVALID &&
                   comp_(nodes_[nodes_[cur].forward[i]].value, value)) {
                cur = nodes_[cur].forward[i];
            }
            update[i] = cur;
        }

        size_t target = nodes_[cur].forward[0];
        if (target == INVALID || comp_(value, nodes_[target].value) ||
            comp_(nodes_[target].value, value)) {
            return false;
        }

        for (size_t i = 0; i < level_; ++i) {
            if (nodes_[update[i]].forward[i] != target) break;
            nodes_[update[i]].forward[i] = nodes_[target].forward[i];
        }

        while (level_ > 1 && nodes_[head_].forward[level_ - 1] == INVALID) {
            --level_;
        }
        --size_;
        return true;
    }

    /// Find first element >= value
    optional<T> lower_bound(const T& value) const {
        size_t cur = head_;
        for (int i = static_cast<int>(level_) - 1; i >= 0; --i) {
            while (nodes_[cur].forward[i] != INVALID &&
                   comp_(nodes_[nodes_[cur].forward[i]].value, value)) {
                cur = nodes_[cur].forward[i];
            }
        }
        size_t next = nodes_[cur].forward[0];
        if (next != INVALID) return optional<T>(nodes_[next].value);
        return optional<T>();
    }

    size_type size() const { return size_; }
    bool empty() const { return size_ == 0; }
    constexpr size_type max_size() const { return N; }

private:
    static const size_t INVALID = static_cast<size_t>(-1);

    struct node {
        T      value;
        size_t forward[MaxLevel];
    };

    /// Deterministic level generation using 16-bit LFSR
    size_t random_level() {
        size_t lvl = 1;
        while (lvl < MaxLevel) {
            lfsr_step();
            if (lfsr_ & 1) break; // ~50% chance to go up
            ++lvl;
        }
        return lvl;
    }

    void lfsr_step() {
        // 16-bit LFSR with taps at positions 16,14,13,11
        uint16_t bit = ((lfsr_ >> 0) ^ (lfsr_ >> 2) ^ (lfsr_ >> 3) ^ (lfsr_ >> 5)) & 1u;
        lfsr_ = static_cast<uint16_t>((lfsr_ >> 1) | (bit << 15));
    }

    node          nodes_[N + 1]; // +1 for head node
    size_t        head_;
    size_type     node_count_;
    size_type     size_;
    size_t        level_;
    uint16_t      lfsr_;
    compare_type  comp_;
};

} // namespace prototype

#endif // PROTOTYPE_ADVANCED_SKIP_LIST_HPP
