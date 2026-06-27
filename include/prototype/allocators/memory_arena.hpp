///////////////////////////////////////////////////////////////////////////////
// include/prototype/allocators/memory_arena.hpp
//
// PrototypeSTL - Fixed-size memory arena (bump allocator)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_MEMORY_ARENA_HPP
#define PROTOTYPE_MEMORY_ARENA_HPP

#include "../config.hpp"

namespace prototype {

class memory_arena {
public:
    memory_arena(void* buffer, size_t size)
        : buffer_(static_cast<char*>(buffer))
        , size_(size)
        , offset_(0) {}

    void* allocate(size_t bytes, size_t alignment = alignof(max_align_t)) {
        size_t aligned_offset = align_up(offset_, alignment);
        if (aligned_offset + bytes > size_) {
            return nullptr;
        }
        void* ptr = buffer_ + aligned_offset;
        offset_ = aligned_offset + bytes;
        return ptr;
    }

    void deallocate(void*, size_t) {
        // bump allocator: individual deallocation is a no-op
    }

    void reset() { offset_ = 0; }
    size_t used() const { return offset_; }
    size_t remaining() const { return size_ - offset_; }
    size_t capacity() const { return size_; }

private:
    static size_t align_up(size_t offset, size_t alignment) {
        return (offset + alignment - 1) & ~(alignment - 1);
    }

    char*  buffer_;
    size_t size_;
    size_t offset_;
};

} // namespace prototype

#endif // PROTOTYPE_MEMORY_ARENA_HPP
