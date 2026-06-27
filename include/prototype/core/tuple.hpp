///////////////////////////////////////////////////////////////////////////////
// include/prototype/core/tuple.hpp
//
// PrototypeSTL - Fixed-size heterogeneous container (tuple)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_CORE_TUPLE_HPP
#define PROTOTYPE_CORE_TUPLE_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"

namespace prototype {

///////////////////////////////////////////////////////////////////////////////
// tuple implementation using recursive inheritance
///////////////////////////////////////////////////////////////////////////////

// Forward declaration
template<class... Types>
class tuple;

// Base case: empty tuple
template<>
class tuple<> {
public:
    tuple() {}
};

// Recursive case
template<class Head, class... Tail>
class tuple<Head, Tail...> : public tuple<Tail...> {
    typedef tuple<Tail...> base_type;
public:
    tuple() : base_type(), value_() {}

    explicit tuple(const Head& head, const Tail&... tail)
        : base_type(tail...), value_(head) {}

    Head& head() { return value_; }
    const Head& head() const { return value_; }

    base_type& tail() { return *this; }
    const base_type& tail() const { return *this; }

private:
    Head value_;
};

///////////////////////////////////////////////////////////////////////////////
// tuple_size
///////////////////////////////////////////////////////////////////////////////
template<class T>
struct tuple_size;

template<class... Types>
struct tuple_size<tuple<Types...>> {
    static const size_t value = sizeof...(Types);
};

///////////////////////////////////////////////////////////////////////////////
// tuple_element
///////////////////////////////////////////////////////////////////////////////
template<size_t I, class T>
struct tuple_element;

template<class Head, class... Tail>
struct tuple_element<0, tuple<Head, Tail...>> {
    typedef Head type;
};

template<size_t I, class Head, class... Tail>
struct tuple_element<I, tuple<Head, Tail...>> {
    typedef typename tuple_element<I - 1, tuple<Tail...>>::type type;
};

///////////////////////////////////////////////////////////////////////////////
// get<I>(tuple)
///////////////////////////////////////////////////////////////////////////////
namespace detail {

template<size_t I, class Head, class... Tail>
struct tuple_getter {
    typedef typename tuple_element<I, tuple<Head, Tail...>>::type return_type;

    static return_type& get(tuple<Head, Tail...>& t) {
        return tuple_getter<I - 1, Tail...>::get(t.tail());
    }

    static const return_type& get(const tuple<Head, Tail...>& t) {
        return tuple_getter<I - 1, Tail...>::get(t.tail());
    }
};

template<class Head, class... Tail>
struct tuple_getter<0, Head, Tail...> {
    static Head& get(tuple<Head, Tail...>& t) {
        return t.head();
    }

    static const Head& get(const tuple<Head, Tail...>& t) {
        return t.head();
    }
};

} // namespace detail

template<size_t I, class... Types>
typename tuple_element<I, tuple<Types...>>::type&
get(tuple<Types...>& t) {
    return detail::tuple_getter<I, Types...>::get(t);
}

template<size_t I, class... Types>
const typename tuple_element<I, tuple<Types...>>::type&
get(const tuple<Types...>& t) {
    return detail::tuple_getter<I, Types...>::get(t);
}

///////////////////////////////////////////////////////////////////////////////
// make_tuple
///////////////////////////////////////////////////////////////////////////////
template<class... Types>
tuple<typename decay<Types>::type...> make_tuple(Types&&... args) {
    return tuple<typename decay<Types>::type...>(prototype::forward<Types>(args)...);
}

///////////////////////////////////////////////////////////////////////////////
// tie
///////////////////////////////////////////////////////////////////////////////
template<class... Types>
tuple<Types&...> tie(Types&... args) {
    return tuple<Types&...>(args...);
}

} // namespace prototype

#endif // PROTOTYPE_CORE_TUPLE_HPP
