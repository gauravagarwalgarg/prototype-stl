///////////////////////////////////////////////////////////////////////////////
// include/prototype/iterators/move_iterator.hpp
//
// PrototypeSTL - Move iterator adaptor (C++11)
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_MOVE_ITERATOR_HPP
#define PROTOTYPE_MOVE_ITERATOR_HPP

#include "../config.hpp"
#include "../utility/utility.hpp"
#include "iterator_traits.hpp"

namespace prototype {

template<class Iterator>
class move_iterator {
public:
    typedef Iterator iterator_type;
    typedef typename iterator_traits<Iterator>::difference_type   difference_type;
    typedef typename iterator_traits<Iterator>::value_type        value_type;
    typedef Iterator                                              pointer;
    typedef value_type&&                                          reference;
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;

    move_iterator() : current_() {}
    explicit move_iterator(Iterator it) : current_(it) {}

    template<class U>
    move_iterator(const move_iterator<U>& other) : current_(other.base()) {}

    Iterator base() const { return current_; }

    reference operator*() const {
        return static_cast<reference>(*current_);
    }

    pointer operator->() const { return current_; }

    move_iterator& operator++() { ++current_; return *this; }
    move_iterator  operator++(int) { move_iterator t(*this); ++current_; return t; }
    move_iterator& operator--() { --current_; return *this; }
    move_iterator  operator--(int) { move_iterator t(*this); --current_; return t; }

    move_iterator operator+(difference_type n) const { return move_iterator(current_ + n); }
    move_iterator operator-(difference_type n) const { return move_iterator(current_ - n); }
    move_iterator& operator+=(difference_type n) { current_ += n; return *this; }
    move_iterator& operator-=(difference_type n) { current_ -= n; return *this; }

    reference operator[](difference_type n) const {
        return prototype::move(current_[n]);
    }

private:
    Iterator current_;
};

template<class It1, class It2>
inline bool operator==(const move_iterator<It1>& a, const move_iterator<It2>& b) {
    return a.base() == b.base();
}
template<class It1, class It2>
inline bool operator!=(const move_iterator<It1>& a, const move_iterator<It2>& b) {
    return a.base() != b.base();
}
template<class It1, class It2>
inline bool operator<(const move_iterator<It1>& a, const move_iterator<It2>& b) {
    return a.base() < b.base();
}

template<class Iterator>
inline move_iterator<Iterator> make_move_iterator(Iterator it) {
    return move_iterator<Iterator>(it);
}

} // namespace prototype

#endif // PROTOTYPE_MOVE_ITERATOR_HPP
