///////////////////////////////////////////////////////////////////////////////
// include/prototype/iterators/iterator_traits.hpp
//
// PrototypeSTL - Iterator tags and traits
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ITERATOR_TRAITS_HPP
#define PROTOTYPE_ITERATOR_TRAITS_HPP

#include "../config.hpp"
#include "../type_traits/type_traits.hpp"

namespace prototype {

///////////////////////////////////////////////////////////////////////////////
// Iterator category tags
///////////////////////////////////////////////////////////////////////////////
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : input_iterator_tag {};
struct bidirectional_iterator_tag : forward_iterator_tag {};
struct random_access_iterator_tag : bidirectional_iterator_tag {};

///////////////////////////////////////////////////////////////////////////////
// iterator_traits
///////////////////////////////////////////////////////////////////////////////
template<class Iterator>
struct iterator_traits {
    typedef typename Iterator::difference_type   difference_type;
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
    typedef typename Iterator::iterator_category iterator_category;
};

// Specialization for pointers
template<class T>
struct iterator_traits<T*> {
    typedef ptrdiff_t                  difference_type;
    typedef T                          value_type;
    typedef T*                         pointer;
    typedef T&                         reference;
    typedef random_access_iterator_tag iterator_category;
};

template<class T>
struct iterator_traits<const T*> {
    typedef ptrdiff_t                  difference_type;
    typedef T                          value_type;
    typedef const T*                   pointer;
    typedef const T&                   reference;
    typedef random_access_iterator_tag iterator_category;
};

} // namespace prototype

#endif // PROTOTYPE_ITERATOR_TRAITS_HPP
