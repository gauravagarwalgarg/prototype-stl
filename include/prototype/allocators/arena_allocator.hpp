///////////////////////////////////////////////////////////////////////////////
// include/prototype/allocators/arena_allocator.hpp
//
// PrototypeSTL - Arena-backed allocator (STL-compatible interface)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ARENA_ALLOCATOR_HPP
#define PROTOTYPE_ARENA_ALLOCATOR_HPP

#include "../config.hpp"
#include "memory_arena.hpp"

namespace prototype {

template<class T>
class arena_allocator {
public:
    typedef T              value_type;
    typedef T*             pointer;
    typedef const T*       const_pointer;
    typedef T&             reference;
    typedef const T&       const_reference;
    typedef size_t         size_type;
    typedef ptrdiff_t      difference_type;

    template<class U>
    struct rebind { typedef arena_allocator<U> other; };

    explicit arena_allocator(memory_arena& arena) : arena_(&arena) {}

    template<class U>
    arena_allocator(const arena_allocator<U>& other) : arena_(other.arena()) {}

    pointer allocate(size_type n) {
        void* p = arena_->allocate(n * sizeof(T), alignof(T));
        PROTOTYPE_ASSERT(p != nullptr);
        return static_cast<pointer>(p);
    }

    void deallocate(pointer, size_type) {
        // arena allocator: deallocation handled by arena reset
    }

    memory_arena* arena() const { return arena_; }

private:
    memory_arena* arena_;
};

template<class T, class U>
inline bool operator==(const arena_allocator<T>& a, const arena_allocator<U>& b) {
    return a.arena() == b.arena();
}

template<class T, class U>
inline bool operator!=(const arena_allocator<T>& a, const arena_allocator<U>& b) {
    return !(a == b);
}

} // namespace prototype

#endif // PROTOTYPE_ARENA_ALLOCATOR_HPP
