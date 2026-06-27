///////////////////////////////////////////////////////////////////////////////
// include/prototype/allocators/stack_allocator.hpp
//
// PrototypeSTL - LIFO stack allocator (bump-up, deallocate only in reverse)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_STACK_ALLOCATOR_HPP
#define PROTOTYPE_STACK_ALLOCATOR_HPP

#include "../config.hpp"

namespace prototype {

template<size_t StackSize>
class stack_allocator_storage {
public:
    stack_allocator_storage() : offset_(0) {}

    void* allocate(size_t bytes, size_t alignment = alignof(max_align_t)) {
        size_t aligned = align_up(offset_, alignment);
        if (aligned + bytes > StackSize) return nullptr;
        void* ptr = buffer_ + aligned;
        offset_ = aligned + bytes;
        return ptr;
    }

    void deallocate(void* ptr, size_t bytes) {
        // Only supports LIFO deallocation
        char* p = static_cast<char*>(ptr);
        if (p + bytes == buffer_ + offset_) {
            offset_ = static_cast<size_t>(p - buffer_);
        }
    }

    void reset() { offset_ = 0; }
    size_t used() const { return offset_; }
    size_t remaining() const { return StackSize - offset_; }

private:
    static size_t align_up(size_t offset, size_t alignment) {
        return (offset + alignment - 1) & ~(alignment - 1);
    }

    PROTOTYPE_ALIGNAS(16) char buffer_[StackSize];
    size_t offset_;
};

template<class T, size_t StackSize = 4096>
class stack_allocator {
public:
    typedef T         value_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef size_t    size_type;
    typedef ptrdiff_t difference_type;

    template<class U>
    struct rebind { typedef stack_allocator<U, StackSize> other; };

    explicit stack_allocator(stack_allocator_storage<StackSize>& storage)
        : storage_(&storage) {}

    template<class U>
    stack_allocator(const stack_allocator<U, StackSize>& other)
        : storage_(other.storage()) {}

    pointer allocate(size_type n) {
        void* p = storage_->allocate(n * sizeof(T), alignof(T));
        PROTOTYPE_ASSERT(p != nullptr);
        return static_cast<pointer>(p);
    }

    void deallocate(pointer p, size_type n) {
        storage_->deallocate(p, n * sizeof(T));
    }

    stack_allocator_storage<StackSize>* storage() const { return storage_; }

private:
    stack_allocator_storage<StackSize>* storage_;
};

} // namespace prototype

#endif // PROTOTYPE_STACK_ALLOCATOR_HPP
