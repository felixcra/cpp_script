/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#include <type_traits>
#include <iterator>
#include <concepts>
#include <vector>

#include "element.hpp"

namespace cs {

/* Type definitions */
template <typename T>
using vector = std::vector<T>;

class Container {
public:

vector<Element> elems_;

friend class Iterator;

class Iterator {
public:

using value_type        = Element;
using difference_type   = int;
using reference         = Element&;
using pointer           = Element*;
using iterator_category = std::input_iterator_tag;

Iterator(Container* c, const int& idx) :
    c_(c),
    idx_(idx)
{}

bool operator==(const Iterator& i) const {
    return idx_ == i.idx_;
}

bool operator!=(const Iterator& i) const {
    return !(*this == i);
}

Iterator& operator++() {
    ++idx_;
    return *this;
}

reference operator*() {
    return c_->elems_.at(idx_);
}

protected:

Container* c_;
int idx_;

};

class ConstIterator {
public:

using value_type        = const Element;
using difference_type   = int;
using reference         = const Element&;
using pointer           = const Element*;
using iterator_category = std::input_iterator_tag;

ConstIterator(const Container* c, const int& idx) : c_(c), idx_(idx) {}

bool operator==(const ConstIterator& i) const {
    return idx_ == i.idx_;
}

bool operator!=(const ConstIterator& i) const {
    return !(*this == i);
}

ConstIterator& operator++() {
    ++idx_;
    return *this;
}

reference operator*() {
    return c_->elems_.at(idx_);
}

protected:

const Container* c_;
int idx_;

};

Iterator begin() {
    return Iterator(this,0);
}

Iterator end() {
    return Iterator(this,elems_.size());
}

ConstIterator begin() const {
    return ConstIterator(this,0);
}

ConstIterator end() const {
    return ConstIterator(this,elems_.size());
}

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

};

};
