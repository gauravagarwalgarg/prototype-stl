///////////////////////////////////////////////////////////////////////////////
// include/prototype/allocators/allocator.hpp
//
// PrototypeSTL - Default allocator<T> using malloc/free
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ALLOCATOR_HPP
#define PROTOTYPE_ALLOCATOR_HPP

#include "../config.hpp"
#include "../type_traits/type_traits.hpp"
#include "../utility/utility.hpp"

namespace prototype {

template<class T>
class allocator {
public:
    typedef T              value_type;
    typedef T*             pointer;
    typedef const T*       const_pointer;
    typedef T&             reference;
    typedef const T&       const_reference;
    typedef size_t         size_type;
    typedef ptrdiff_t      difference_type;

    template<class U>
    struct rebind { typedef allocator<U> other; };

    allocator() {}
    template<class U> allocator(const allocator<U>&) {}

    pointer allocate(size_type n) {
        void* p = ::malloc(n * sizeof(T));
        PROTOTYPE_ASSERT(p != nullptr);
        return static_cast<pointer>(p);
    }

    void deallocate(pointer p, size_type) {
        ::free(p);
    }

    size_type max_size() const {
        return size_type(-1) / sizeof(T);
    }

    template<class U, class... Args>
    void construct(U* p, Args&&... args) {
        ::new(static_cast<void*>(p)) U(prototype::forward<Args>(args)...);
    }

    template<class U>
    void destroy(U* p) {
        p->~U();
    }
};

template<class T, class U>
inline bool operator==(const allocator<T>&, const allocator<U>&) { return true; }

template<class T, class U>
inline bool operator!=(const allocator<T>&, const allocator<U>&) { return false; }

} // namespace prototype

#endif // PROTOTYPE_ALLOCATOR_HPP
