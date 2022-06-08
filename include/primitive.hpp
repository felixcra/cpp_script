/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#include <type_traits>
#include <string>

#include "object.hpp"

namespace cs {

/* Type definitions */
using string = std::string;

template <typename T>
requires std::is_arithmetic_v<T>
class Primitive : public Object {

public:

/* Constructors */
explicit Primitive(const T& v) : v_(v) {}

explicit Primitive(const Primitive<T>& p) : v_(p.v_) {}

/* Destructor */
~Primitive() override {
    OBJECT_DESCTRUCT(this);
}

/* Assignment */
Primitive& operator=(T&& v) {
    v_ = v;

    return *this;
}

bool is_equal(const Object* o) const override {
    return dynamic_cast<const Primitive<T>*>(o) != nullptr &&
           dynamic_cast<const Primitive<T>*>(o)->v_ == v_;
}

string to_string() const override {
    return std::to_string(v_);
}

private:

T v_;

};

};
