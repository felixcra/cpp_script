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
#include <vector>

#ifdef DEBUG_ELEMENT
#include <iostream>
#endif

#include "object.hpp"
#include "primitives.hpp"
#include "string.hpp"
#include "iterable.hpp"

namespace cs {

/* Type definitions */
template <typename T>
using shared_ptr = std::shared_ptr<T>;
using exception  = std::exception;
using string     = std::string;
template <typename T>
using vector     = std::vector<T>;

template <typename T>
struct is_nested_vector_or_arithmetic_trait : std::false_type { };

template <typename T>
concept is_arithmetic = std::is_arithmetic_v<T>;

template <is_arithmetic T>
struct is_nested_vector_or_arithmetic_trait<T> : std::true_type { };

template <typename T>
struct is_nested_vector_or_arithmetic_trait<vector<T>> : is_nested_vector_or_arithmetic_trait<T> { };

template <typename T>
concept is_nested_vector_or_arithmetic = is_nested_vector_or_arithmetic_trait<T>::value;

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
    if (clone_primitives && dynamic_cast<const Primitive*>(e.r_.c_get()) != nullptr) {
        Primitive* p = dynamic_cast<const Primitive*>(e.r_.c_get())->clone();
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
          std::is_same_v<std::remove_reference_t<T>,bool> ||
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
            if constexpr (std::is_same_v<T__,bool>) {
                Bool* b = new Bool(v);
                r_.set(*b,true);
            } else if constexpr (std::is_integral_v<T__>) {
                Int* i = new Int(v);
                r_.set(*i,true);
            } else {
                Float* f = new Float(v);
                r_.set(*f,true);
            }
        }
    }
#ifdef DEBUG_ELEMENT
    std::cout << "Element(T&& v) : this = " << (void*) this << " r_.o_ " << (void*) r_.o_ << std::endl;
#endif
}

explicit Element(const char* s) : Element(string(s))
{}

Element& operator=(const Element& e) {
    r_ = e.r_;

    return *this;
}

template <typename T>
requires (std::is_base_of_v<Object,std::remove_reference_t<T>> || 
          std::is_same_v<std::remove_reference_t<T>,bool> ||
          std::is_arithmetic_v<std::remove_reference_t<T>> ||
          std::is_same_v<std::remove_cvref_t<T>,string>)
Element& operator=(T&& v) {
    using T_ = std::remove_cvref_t<T>;

    if constexpr (std::is_base_of_v<Object,T_>) {
        // Create a new object and a reference to it
        T_* t;
        if constexpr (std::is_same_v<T_,Object>) {
            t = new T_();
            t->registry_ = std::move(v.registry_);
        } else {
            t = new T_(std::move(v));
        }
        r_.set(*t,true);

        return *this;
    } else {
        if constexpr (std::is_same_v<T_,string>) {
            if (dynamic_cast<String*>(r_.get()) != nullptr) {
                *dynamic_cast<String*>(r_.get()) = v;
            } else {
                // Create a String and a reference to it
                String* s = new String(v);
                r_.set(*s,true);
            }
        } else if constexpr (std::is_same_v<T_,bool>) {
            if (dynamic_cast<Bool*>(r_.get()) != nullptr) {
                *dynamic_cast<Bool*>(r_.get()) = v;
            } else {
                // Create a Bool and a reference to it
                Bool* b = new Bool(v);
                r_.set(*b,true);
            }
        } else if constexpr (std::is_integral_v<T_>) {
            if (dynamic_cast<Int*>(r_.get()) != nullptr) {
                *dynamic_cast<Int*>(r_.get()) = v;
            } else {
                // Create an Int and a reference to it
                Int* i = new Int(v);
                r_.set(*i,true);
            }
        } else {
            if (dynamic_cast<Float*>(r_.get()) != nullptr) {
                *dynamic_cast<Float*>(r_.get()) = v;
            } else {
                // Create a Float and a reference to it
                Float* f = new Float(v);
                r_.set(*f,true);
            }
        }

        return *this;
    }
}

template <is_nested_vector_or_arithmetic T>
Element& operator=(const vector<T>& v);

Element& operator=(const char* s) {
    return operator=(string(s));
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
requires (std::is_arithmetic_v<std::remove_reference_t<T>> ||
          std::is_same_v<std::remove_reference_t<T>,bool>)
bool operator==(T&& v) const {
    assert(r_.v_);

    using T_ = std::remove_cvref_t<T>;

    if constexpr (std::is_same_v<T_,bool>) {
        const Bool b(v);
        return r_.o_->is_equal(&b);
    } else if constexpr (std::is_integral_v<T_>) {
        const Int i(v);
        return r_.o_->is_equal(&i);
    } else {
        const Float f(v);
        return r_.o_->is_equal(&f);
    }
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
bool isinstance() const {
    return dynamic_cast<const T*>(r_.c_get()) != nullptr;
}

template <typename T>
requires std::is_base_of_v<Object,T>
T& as() {
    if (dynamic_cast<T*>(r_.get()) == nullptr) {
        throw InvalidConversionException("Invalid conversion of Element requested.");
    }
    return *dynamic_cast<T*>(r_.get());
}

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

inline std::ostream& operator<<(std::ostream& out, const Element& e) {
    return out << e.to_string();
}

template <typename T>
inline bool isinstance(const Element& e) {
    return e.isinstance<T>();
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
