/* 
 * Author: Felix Crazzolara
 */ 

#pragma once

#include <memory>
#include <exception>
#include <string>
#include <type_traits>
#include <ostream>

#include "object.hpp"
#include "primitive.hpp"

namespace cs {

/* Type definitions */
template <typename T>
using shared_ptr = std::shared_ptr<T>;
using exception  = std::exception;
using string     = std::string;

class Element {
public:

class InvalidConversionException : public exception {
public:

explicit InvalidConversionException(const string& s) : s_(s) {}

const char* what() const noexcept override {
    return s_.c_str();
}

private:

const string s_;

};

/* Constructors and assignment */
Element() = delete;

Element(const Element& e) : r_(e.r_) {}

template <typename T>
requires std::is_base_of_v<Object,std::remove_reference_t<T>> || std::is_arithmetic_v<T>
Element(T&& v) {
    static_assert(!(std::is_lvalue_reference_v<decltype(std::forward<T>(v))> && std::is_arithmetic_v<T>));

    if constexpr (std::is_lvalue_reference_v<decltype(std::forward<T>(v))>) {
        // Create a reference to the object
        r_.set(v,false);
    } else {
        if constexpr (std::is_class_v<T>) {
            // Create a new object and a reference to it
            T* t;
            if constexpr (std::is_same_v<T,Object>) {
                t = new T();
                t->registry_ = std::move(v.registry_);
            } else {
                t = new T(std::move(v));
            }
            r_.set(*t,true);
        } else {
            // Create a primitive and a reference to it
            Primitive<T>* p = new Primitive<T>(v);
            r_.set(*p,true);
        }
    }
}

Element& operator=(const Element& e) {
    r_ = e.r_;

    return *this;
}

/* Boolean operators */
bool operator==(const Element& e) const {
    assert(r_.v_ && e.r_.v_);

    return r_.o_->is_equal(e.r_.o_);
}

bool operator!=(const Element& e) const {
    return !(*this == e);
}

/* Miscellaneous */
template <typename T>
requires std::is_base_of_v<Object,T>
const T& as() const {
    if (dynamic_cast<const T*>(r_.get()) == nullptr) {
        throw InvalidConversionException("Invalid conversion of Element requested.");
    }

    return *dynamic_cast<const T*>(r_.get());
}

string to_string() const {
    return r_.get()->to_string();
}

private:

ObjectRef r_;

friend std::ostream& operator<<(std::ostream& out, const Element& e);

};

std::ostream& operator<<(std::ostream& out, const Element& e) {
    return out << e.to_string();
}

};
