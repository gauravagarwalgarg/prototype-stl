///////////////////////////////////////////////////////////////////////////////
// include/prototype/functors/function.hpp
//
// PrototypeSTL - fixed_function (type-erased callable) and delegate
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_FUNCTORS_FUNCTION_HPP
#define PROTOTYPE_FUNCTORS_FUNCTION_HPP

#include "../config.hpp"
#include "../type_traits/type_traits.hpp"
#include "../utility/utility.hpp"

namespace prototype {

///////////////////////////////////////////////////////////////////////////////
// fixed_function - type-erased callable with inline storage (no heap)
///////////////////////////////////////////////////////////////////////////////
template<class Signature, size_t StorageSize = 32>
class fixed_function; // primary template not defined

template<class Ret, class... Args, size_t StorageSize>
class fixed_function<Ret(Args...), StorageSize> {
    typedef Ret (*invoke_fn)(void*, Args...);
    typedef void (*destroy_fn)(void*);
    typedef void (*copy_fn)(void*, const void*);

public:
    fixed_function() : invoke_(nullptr), destroy_(nullptr), copy_(nullptr) {}

    template<class F>
    fixed_function(F f) : invoke_(nullptr), destroy_(nullptr), copy_(nullptr) {
        static_assert(sizeof(F) <= StorageSize, "Callable too large for fixed_function storage");
        ::new(static_cast<void*>(&storage_)) F(prototype::move(f));
        invoke_ = &invoke_impl<F>;
        destroy_ = &destroy_impl<F>;
        copy_ = &copy_impl<F>;
    }

    fixed_function(const fixed_function& other) : invoke_(other.invoke_), destroy_(other.destroy_), copy_(other.copy_) {
        if (copy_) copy_(&storage_, &other.storage_);
    }

    fixed_function& operator=(const fixed_function& other) {
        if (this != &other) {
            destroy_stored();
            invoke_ = other.invoke_;
            destroy_ = other.destroy_;
            copy_ = other.copy_;
            if (copy_) copy_(&storage_, &other.storage_);
        }
        return *this;
    }

    ~fixed_function() { destroy_stored(); }

    explicit operator bool() const { return invoke_ != nullptr; }

    Ret operator()(Args... args) {
        PROTOTYPE_ASSERT(invoke_ != nullptr);
        return invoke_(&storage_, static_cast<Args>(args)...);
    }

    void reset() { destroy_stored(); invoke_ = nullptr; destroy_ = nullptr; copy_ = nullptr; }

private:
    void destroy_stored() {
        if (destroy_) { destroy_(&storage_); destroy_ = nullptr; }
    }

    template<class F>
    static Ret invoke_impl(void* storage, Args... args) {
        return (*static_cast<F*>(storage))(static_cast<Args>(args)...);
    }

    template<class F>
    static void destroy_impl(void* storage) {
        static_cast<F*>(storage)->~F();
    }

    template<class F>
    static void copy_impl(void* dst, const void* src) {
        ::new(dst) F(*static_cast<const F*>(src));
    }

    typename aligned_storage<StorageSize, alignof(max_align_t)>::type storage_;
    invoke_fn  invoke_;
    destroy_fn destroy_;
    copy_fn    copy_;
};

///////////////////////////////////////////////////////////////////////////////
// delegate - lightweight non-owning function reference (function pointer + context)
///////////////////////////////////////////////////////////////////////////////
template<class Signature>
class delegate;

template<class Ret, class... Args>
class delegate<Ret(Args...)> {
    typedef Ret (*stub_fn)(void*, Args...);

public:
    delegate() : object_(nullptr), stub_(nullptr) {}

    template<class T, Ret (T::*Method)(Args...)>
    static delegate from(T* obj) {
        delegate d;
        d.object_ = obj;
        d.stub_ = &method_stub<T, Method>;
        return d;
    }

    template<Ret (*Function)(Args...)>
    static delegate from() {
        delegate d;
        d.object_ = nullptr;
        d.stub_ = &function_stub<Function>;
        return d;
    }

    Ret operator()(Args... args) const {
        PROTOTYPE_ASSERT(stub_ != nullptr);
        return stub_(object_, static_cast<Args>(args)...);
    }

    explicit operator bool() const { return stub_ != nullptr; }

private:
    template<class T, Ret (T::*Method)(Args...)>
    static Ret method_stub(void* obj, Args... args) {
        return (static_cast<T*>(obj)->*Method)(static_cast<Args>(args)...);
    }

    template<Ret (*Function)(Args...)>
    static Ret function_stub(void*, Args... args) {
        return Function(static_cast<Args>(args)...);
    }

    void*   object_;
    stub_fn stub_;
};

} // namespace prototype

#endif // PROTOTYPE_FUNCTORS_FUNCTION_HPP
