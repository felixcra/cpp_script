/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#ifdef DEBUG_ITERABLE
#include <iostream>
#endif

namespace cs {

template <typename U>
class Iterable {
protected:

virtual void iter_init() const = 0;

virtual void iter_inc() const = 0;

virtual uint iter_max() const = 0;

virtual U& iter_get() = 0;

virtual const U& c_iter_get() const = 0;

class Iterator {
public:

using value_type        = U;
using difference_type   = int;
using reference         = U&;
using pointer           = U*;
using iterator_category = std::input_iterator_tag;

Iterator(Iterable* i, const int& idx) :
    i_(i),
    idx_(idx)
{
    i->iter_init();
}

bool operator==(const Iterator& i) const {
    return idx_ == i.idx_;
}

bool operator!=(const Iterator& i) const {
    return !(*this == i);
}

Iterator& operator++() {
    i_->iter_inc();
    ++idx_;
    return *this;
}

reference operator*() {
    return i_->iter_get();
}

protected:

Iterable* i_;
int idx_;

};

class ConstIterator {
public:

using value_type        = const U;
using difference_type   = int;
using reference         = const U&;
using pointer           = const U*;
using iterator_category = std::input_iterator_tag;

ConstIterator(const Iterable* i, const int& idx) :
    i_(i),
    idx_(idx)
{
#ifdef DEBUG_ITERABLE
    std::cout << "ConstIterator(const Iterable* i, const int& idx) : this = " << (void*) this << std::endl;
#endif
    i->iter_init();
}

bool operator==(const ConstIterator& i) const {
    return idx_ == i.idx_;
}

bool operator!=(const ConstIterator& i) const {
    return !(*this == i);
}

ConstIterator& operator++() {
    i_->iter_inc();
    ++idx_;
    return *this;
}

reference operator*() {
#ifdef DEBUG_ITERABLE
    std::cout << "ConstIterator::operator*() : this = " << (void*) this << std::endl;
#endif
    return i_->c_iter_get();
}

protected:

const Iterable* i_;
int idx_;

};

static_assert(std::is_copy_constructible_v<Iterator>);
static_assert(std::is_copy_assignable_v<Iterator>);
static_assert(std::is_destructible_v<Iterator>);
static_assert(std::is_swappable_v<Iterator>);
static_assert(std::equality_comparable<Iterator>);

static_assert(std::is_copy_constructible_v<ConstIterator>);
static_assert(std::is_copy_assignable_v<ConstIterator>);
static_assert(std::is_destructible_v<ConstIterator>);
static_assert(std::is_swappable_v<ConstIterator>);
static_assert(std::equality_comparable<ConstIterator>);

public:

Iterator begin() {
    return Iterator(this,0);
}

ConstIterator begin() const {
#ifdef DEBUG_ITERABLE
    std::cout << "Iterable::begin() const : this = " << (void*) this << std::endl;
#endif
    return ConstIterator(this,0);
}

Iterator end() {
    return Iterator(this,iter_max());
}

ConstIterator end() const {
#ifdef DEBUG_ITERABLE
    std::cout << "Iterable::end() const : this = " << (void*) this << std::endl;
#endif
    return ConstIterator(this,iter_max());
}

};

};
