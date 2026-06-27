///////////////////////////////////////////////////////////////////////////////
// include/prototype/allocators/pool_allocator.hpp
//
// PrototypeSTL - Fixed-size pool allocator (free-list based)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_POOL_ALLOCATOR_HPP
#define PROTOTYPE_POOL_ALLOCATOR_HPP

#include "../config.hpp"

namespace prototype {

template<class T, size_t PoolSize = 64>
class pool_allocator {
public:
    typedef T         value_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef size_t    size_type;
    typedef ptrdiff_t difference_type;

    template<class U>
    struct rebind { typedef pool_allocator<U, PoolSize> other; };

    pool_allocator() : free_list_(nullptr) {
        init_pool();
    }

    pointer allocate(size_type n) {
        PROTOTYPE_ASSERT(n == 1); // pool allocator: single-element allocation only
        PROTOTYPE_ASSERT(free_list_ != nullptr); // pool exhausted
        node* block = free_list_;
        free_list_ = free_list_->next;
        return reinterpret_cast<pointer>(block);
    }

    void deallocate(pointer p, size_type) {
        node* block = reinterpret_cast<node*>(p);
        block->next = free_list_;
        free_list_ = block;
    }

    size_type max_size() const { return PoolSize; }

private:
    union node {
        node* next;
        unsigned char storage[sizeof(T) > sizeof(node*) ? sizeof(T) : sizeof(node*)];
    };

    void init_pool() {
        for (size_t i = 0; i < PoolSize; ++i) {
            pool_[i].next = (i + 1 < PoolSize) ? &pool_[i + 1] : nullptr;
        }
        free_list_ = &pool_[0];
    }

    node  pool_[PoolSize];
    node* free_list_;
};

} // namespace prototype

#endif // PROTOTYPE_POOL_ALLOCATOR_HPP
