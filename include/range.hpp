/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#include <string>
#include <memory>

#include "object.hpp"
#include "iterable.hpp"

namespace cs {

/* Type definitions */
using uint       = unsigned int;
using string     = std::string;
template <typename T>
using unique_ptr = std::unique_ptr<T>;

class Range : public Iterable<uint>, public virtual Object {
public:

/* Constructors */
explicit Range(const uint u) :
    l_(0),
    u_(u),
    v_ptr_(new uint)
{}

explicit Range(const uint l, const uint u) :
    l_(l),
    u_(u),
    v_ptr_(new uint)
{}

explicit Range(const Range& r) :
    l_(r.l_),
    u_(r.u_),
    v_ptr_(new uint)
{}

/* Destructor */
~Range() override {
    OBJECT_DESCTRUCT(this);
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
    return u_ - l_;
}

uint& iter_get() override {
    return *v_ptr_;
}

const uint& c_iter_get() const override {
    return *v_ptr_;
}

private:

const uint             l_;
const uint             u_;
const unique_ptr<uint> v_ptr_;

};

};
