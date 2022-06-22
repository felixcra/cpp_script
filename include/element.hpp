/* 
 * Author: Felix Crazzolara
 */ 

#pragma once

#include <memory>
#include <exception>
#include <string>
#include <type_traits>
#include <ostream>
#include <functional>

#ifdef DEBUG_ELEMENT
#include <iostream>
#endif

#include "object.hpp"
#include "primitive.hpp"
#include "string.hpp"

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

Element(const Element& e, const bool clone_primitives = false) {
#ifdef DEBUG_ELEMENT
    std::cout << "Element(const Element& e) : this = " << (void*) this << " e.this = " << (void*) &e << " r_.o_ = " << (void*) r_.o_ << std::endl;
#endif
    if (clone_primitives && dynamic_cast<const PrimitiveBase*>(e.r_.c_get()) != nullptr) {
        PrimitiveBase* p = dynamic_cast<const PrimitiveBase*>(e.r_.c_get())->clone();
        r_.set(*p,true);
    } else {
        r_ = e.r_;
    }
}

#ifdef DEBUG_ELEMENT
~Element() {
    std::cout << "~Element() : this = " << (void*) this << " r_.o_ = " << (void*) r_.o_;
    std::cout << " r_.o_->registry_->refs_.size() " << r_.o_->registry_->refs_.size() << std::endl;
}
#endif

template <typename T>
requires (std::is_base_of_v<Object,std::remove_reference_t<T>> ||
          std::is_arithmetic_v<std::remove_reference_t<T>> ||
          std::is_same_v<T,string>)
explicit Element(T&& v) {
    using T_ = std::remove_reference_t<T>;

    if constexpr (std::is_lvalue_reference_v<decltype(std::forward<T>(v))> && 
                  std::is_base_of_v<Object,T_>) {
        // Create a reference to the object
        r_.set(v,false);
    } else {
        if constexpr (std::is_same_v<T,string>) {
            // Create a string object a reference to it
            String* s = new String(v);
            r_.set(*s,true);
        } else if constexpr (std::is_class_v<T>) {
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
            using T__ = std::remove_const_t<T_>;

            // Create a primitive and a reference to it
            Primitive<T__>* p = new Primitive<T__>(v);
            r_.set(*p,true);
        }
    }
#ifdef DEBUG_ELEMENT
    std::cout << "Element(T&& v) : this = " << (void*) this << " r_.o_ " << (void*) r_.o_ << std::endl;
#endif
}

template <typename T>
requires (std::is_base_of_v<Object,std::remove_reference_t<T>> || 
          std::is_arithmetic_v<std::remove_reference_t<T>>)
Element& operator=(T&& v) {
    using T_ = std::remove_cvref_t<T>;

    if constexpr (std::is_base_of_v<Object,T_>) {
        // Create a new object and a reference to it
        T* t;
        if constexpr (std::is_same_v<T,Object>) {
            t = new T();
            t->registry_ = std::move(v.registry_);
        } else {
            t = new T(std::move(v));
        }
        r_.set(*t,true);

        return *this;
    } else {
        if (dynamic_cast<Primitive<T_>*>(r_.get()) != nullptr) {
            *dynamic_cast<Primitive<T_>*>(r_.get()) = std::forward<T>(v);
        } else {
            // Create a primitive and a reference to it
            Primitive<T_>* p = new Primitive<T_>(v);
            r_.set(*p,true);
        }
        return *this;
    }
}

explicit Element(const char* s) : Element(string(s))
{}

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

bool operator==(const string& s) const {
    assert(r_.v_);

    String s_(s);

    return r_.o_->is_equal(&s_);
}

bool operator!=(const string& s) const {
    return !(*this == s);
}

template <typename T>
requires std::is_arithmetic_v<std::remove_reference_t<T>>
bool operator==(T&& v) const {
    assert(r_.v_);

    using T_ = std::remove_cvref_t<T>;

    const Primitive<T_> p(v);
    return r_.o_->is_equal(&p);
}

template <typename T>
requires std::is_arithmetic_v<std::remove_reference_t<T>>
bool operator!=(T&& v) const {
    return !(*this == std::forward<T>(v));
}

template <typename T>
requires std::is_base_of_v<Object,std::remove_reference_t<T>>
bool operator==(T&& v) const {
    assert(r_.v_);

    return r_.o_->is_equal(&v);
}

template <typename T>
requires std::is_base_of_v<Object,std::remove_reference_t<T>>
bool operator!=(T&& v) const {
    return !(*this == v);
}

/* Miscellaneous */
template <typename T>
requires std::is_base_of_v<Object,T>
const T& as() const {
    if (dynamic_cast<const T*>(r_.c_get()) == nullptr) {
        throw InvalidConversionException("Invalid conversion of Element requested.");
    }

    return *dynamic_cast<const T*>(r_.c_get());
}

string to_string() const {
    return r_.c_get()->to_string();
}

const ObjectRef& get() const {
    return r_;
}

size_t hash() const {
    return r_.c_get()->hash();
}

private:

ObjectRef r_;

friend std::ostream& operator<<(std::ostream& out, const Element& e);

};

std::ostream& operator<<(std::ostream& out, const Element& e) {
    return out << e.to_string();
}

template <typename T>
bool isinstance(const Element& e) {
    return dynamic_cast<const T*>(e.get().c_get());
}

};

namespace std {

template <>
struct hash<cs::Element> {

size_t operator()(const cs::Element& e) const {
    return e.get().c_get()->hash();
}

};

};
