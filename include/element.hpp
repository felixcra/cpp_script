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

class ExceptionBase : public exception {
public:

explicit ExceptionBase(const string& s) : s_(s) {}

const char* what() const noexcept override {
    return s_.c_str();
}

private:

const string s_;

};

class InvalidArgumentException : public ExceptionBase {
public:

explicit InvalidArgumentException(const string& s) : ExceptionBase(s) {}

};

class InvalidConversionException : public ExceptionBase {
public:

explicit InvalidConversionException(const string& s) : ExceptionBase(s) {}

};

Element() = delete;

explicit Element(Element& e) : Element(static_cast<const Element&>(e)) {}

explicit Element(const Element& e) : r_(e.r_) {}

explicit Element(Element&& e) : r_(std::move(e.r_)) {}

explicit Element(const Element&& e) : r_(e.r_) {}

Element& operator=(const Element& e) {
    r_ = e.r_;

    return *this;
}

template <typename T>
requires std::is_class_v<T> && std::is_polymorphic_v<T>
explicit Element(const T& v) {
    if (std::is_class<T>::value) {
        // Check that T inherits from object
        if constexpr (!std::is_polymorphic<T>::value) {
            throw InvalidArgumentException("Cannot initialize Element with l-value class reference of "
                                           "non-polymorphic type.");
        } else if constexpr (!std::is_base_of<Object,T>::value) {
            throw InvalidArgumentException("Cannot initialize Element with l-value class reference "
                                           "not inheriting from Object.");
        } else {
            // Create a reference to the object
            r_.set(v,false);
        }
    } else {
        throw InvalidArgumentException("Cannot initialize Element with l-value reference of non-class "
                                       "type.");
    }
}

private:

template <typename T>
requires std::is_base_of_v<Object,T> || std::is_arithmetic_v<T>
void set_from_r_value(T&& v) {
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

public:

template <typename T>
requires std::is_base_of_v<Object,T> || std::is_arithmetic_v<T>
explicit Element(T&& v) {
    set_from_r_value(std::move(v));
}

template <typename T>
requires std::is_base_of_v<Object,T> || std::is_arithmetic_v<T>
Element& operator=(T&& v) {
    set_from_r_value(std::move(v));
    return *this;
}

template <typename T>
const T& as() const {
    static_assert(std::is_base_of<Object,T>::value);

    if (dynamic_cast<const T*>(r_.get()) == nullptr) {
        throw InvalidConversionException("Invalid conversion of Element requested.");
    }

    return *dynamic_cast<const T*>(r_.get());
}

bool operator==(const Element& e) const {
    assert(r_.v_ && e.r_.v_);

    return r_.o_->is_equal(e.r_.o_);
}

bool operator!=(const Element& e) const {
    return !(*this == e);
}

private:

ObjectRef r_;

friend std::ostream& operator<<(std::ostream& out, const Element& e);

};

std::ostream& operator<<(std::ostream& out, const Element& e) {
    out << e.r_.get()->to_string();

    return out;
}

};
