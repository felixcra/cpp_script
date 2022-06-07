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
class Primitive : public Object {
static_assert(std::is_arithmetic_v<T>);

public:

explicit Primitive(const T& v) : v_(v) {}

explicit Primitive(const Primitive<T>& p) : v_(p.v_) {}

~Primitive() override {
    OBJECT_DESCTRUCT(this);
}

bool is_equal(const Object* o) const override {
    return dynamic_cast<const Primitive<T>*>(o) != nullptr &&
           dynamic_cast<const Primitive<T>*>(o)->v_ == v_;
}

virtual string to_string() const {
    return std::to_string(v_);
}

private:

const T v_;

};

};
