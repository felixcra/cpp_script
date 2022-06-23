/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#include <string>
#include <memory>

#ifdef DEBUG_RANGE
#include <iostream>
#endif

#include "object.hpp"
#include "iterable.hpp"
#include "primitives.hpp"

namespace cs {

/* Type definitions */
using string     = std::string;
template <typename T>
using unique_ptr = std::unique_ptr<T>;

class Range : public Iterable<int>, public virtual Object {
public:

/* Constructors */
explicit Range(const int u) :
    l_(0),
    u_(u),
    v_ptr_(new int)
{
#ifdef DEBUG_RANGE
    std::cout << "Range(const int u) : this = " << (void*) this << std::endl;
#endif
}

explicit Range(const Int& i) :
    l_(0),
    u_(i.get()),
    v_ptr_(new int)
{
#ifdef DEBUG_RANGE
    std::cout << "Range(const Int& i) : this = " << (void*) this << std::endl;
#endif
}

explicit Range(const int l, const int u) :
    l_(l),
    u_(u),
    v_ptr_(new int)
{
#ifdef DEBUG_RANGE
    std::cout << "Range(const int l, const int u) : this = " << (void*) this << std::endl;
#endif
}

explicit Range(const Range& r) :
    l_(r.l_),
    u_(r.u_),
    v_ptr_(new int)
{
#ifdef DEBUG_RANGE
    std::cout << "Range(const Range& r) : this = " << (void*) this << std::endl;
#endif
}

/* Destructor */
~Range() override {
#ifdef DEBUG_RANGE
    std::cout << "Entering ~Range() : this = " << (void*) this << std::endl;
#endif
    OBJECT_DESCTRUCT(this);
#ifdef DEBUG_RANGE
    std::cout << "Leaving ~Range() : this = " << (void*) this << std::endl;
#endif
}

/* Miscellaneous */
bool is_equal(const Object* o) const override {
    return dynamic_cast<const Range*>(o) != nullptr &&
           dynamic_cast<const Range*>(o)->l_ == l_ &&
           dynamic_cast<const Range*>(o)->u_ == u_;
}

string to_string() const override {
    return "Range("+std::to_string(l_)+","+std::to_string(u_)+")";
}

protected:

void iter_init() const override {
    *v_ptr_ = l_;
}

void iter_inc() const override {
    ++*v_ptr_;
}

uint iter_max() const override {
    return std::max(u_ - l_,0);
}

int& iter_get() override {
    return *v_ptr_;
}

const int& c_iter_get() const override {
    return *v_ptr_;
}

private:

const int             l_;
const int             u_;
const unique_ptr<int> v_ptr_;

};

};
