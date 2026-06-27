///////////////////////////////////////////////////////////////////////////////
// include/prototype/iterators/reverse_iterator.hpp
//
// PrototypeSTL - Reverse iterator adaptor
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_REVERSE_ITERATOR_HPP
#define PROTOTYPE_REVERSE_ITERATOR_HPP

#include "../config.hpp"
#include "iterator_traits.hpp"

namespace prototype {

template<class Iterator>
class reverse_iterator {
public:
    typedef Iterator iterator_type;
    typedef typename iterator_traits<Iterator>::difference_type   difference_type;
    typedef typename iterator_traits<Iterator>::value_type        value_type;
    typedef typename iterator_traits<Iterator>::pointer           pointer;
    typedef typename iterator_traits<Iterator>::reference         reference;
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;

    reverse_iterator() : current_() {}
    explicit reverse_iterator(Iterator x) : current_(x) {}

    template<class U>
    reverse_iterator(const reverse_iterator<U>& other) : current_(other.base()) {}

    Iterator base() const { return current_; }

    reference operator*() const {
        Iterator tmp = current_;
        --tmp;
        return *tmp;
    }

    pointer operator->() const {
        Iterator tmp = current_;
        --tmp;
        return &(*tmp);
    }

    reverse_iterator& operator++() { --current_; return *this; }
    reverse_iterator  operator++(int) { reverse_iterator t(*this); --current_; return t; }
    reverse_iterator& operator--() { ++current_; return *this; }
    reverse_iterator  operator--(int) { reverse_iterator t(*this); ++current_; return t; }

    reverse_iterator operator+(difference_type n) const { return reverse_iterator(current_ - n); }
    reverse_iterator operator-(difference_type n) const { return reverse_iterator(current_ + n); }
    reverse_iterator& operator+=(difference_type n) { current_ -= n; return *this; }
    reverse_iterator& operator-=(difference_type n) { current_ += n; return *this; }

    reference operator[](difference_type n) const { return *(*this + n); }

private:
    Iterator current_;
};

template<class It1, class It2>
inline bool operator==(const reverse_iterator<It1>& a, const reverse_iterator<It2>& b) {
    return a.base() == b.base();
}
template<class It1, class It2>
inline bool operator!=(const reverse_iterator<It1>& a, const reverse_iterator<It2>& b) {
    return a.base() != b.base();
}
template<class It1, class It2>
inline bool operator<(const reverse_iterator<It1>& a, const reverse_iterator<It2>& b) {
    return a.base() > b.base();
}
template<class It1, class It2>
inline bool operator>(const reverse_iterator<It1>& a, const reverse_iterator<It2>& b) {
    return a.base() < b.base();
}

} // namespace prototype

#endif // PROTOTYPE_REVERSE_ITERATOR_HPP
