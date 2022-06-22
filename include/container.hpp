/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#include <type_traits>
#include <iterator>
#include <concepts>
#include <vector>
#include <memory>

#ifdef DEBUG_CONTAINER
#include <iostream>
#endif

#include "element.hpp"
#include "iterable.hpp"

namespace cs {

/* Type definitions */
template <typename T>
using vector     = std::vector<T>;
template <typename T>
using unique_ptr = std::unique_ptr<T>;

class Container : public Iterable<Element> {
public:

Container() : c_ptr_(new uint) {}

protected:

vector<Element> elems_;

void iter_init() const override {
#ifdef DEBUG_LIST
    std::cout << "Container::iter_init()" << std::endl;
#endif
    *c_ptr_ = 0;
}

void iter_inc() const override {
#ifdef DEBUG_LIST
    std::cout << "Container::iter_inc() *c_ptr_" << *c_ptr_ << " -> " << *c_ptr_+1 << std::endl;
#endif
    ++*c_ptr_;
}

uint iter_max() const override {
#ifdef DEBUG_LIST
    std::cout << "Container::iter_max()" << std::endl;
#endif
    return elems_.size();
}

Element& iter_get() override {
#ifdef DEBUG_LIST
    std::cout << "Container::iter_get()" << std::endl;
#endif
    return elems_.at(*c_ptr_);
}

const Element& c_iter_get() const override {
#ifdef DEBUG_LIST
    std::cout << "Container::c_iter_get()" << std::endl;
#endif
    return elems_.at(*c_ptr_);
}

private:

const unique_ptr<uint> c_ptr_;

};

};
