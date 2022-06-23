/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#include <type_traits>
#include <string>
#include <functional>
#include <cstdint>

#ifdef DEBUG_PRIMITIVE
#include <iostream>
#endif

#include "object.hpp"

namespace cs {

/* Type definitions */
using string = std::string;

class Primitive : public virtual Object {
public:

/* Constructors */
Primitive() {}

explicit Primitive(const Primitive& p) {
#ifdef DEBUG_PRIMITIVE
    std::cout << "Primitive(const Primitive& p) : this = " << (void*) this << std::endl;
#endif
}

/* Destructor */
~Primitive() override {
#ifdef DEBUG_PRIMITIVE
    std::cout << "Entering ~Primitive() : this = " << (void*) this << std::endl;
    std::cout << "Not calling OBJECT_DESCTRUCT as Primitive is abstract." << std::endl;
    std::cout << "Leaving ~Primitive() : this = " << (void*) this << std::endl;
#endif
}

/* Miscellanous */
string to_string() const override {
    return "Primitive()";
}

virtual Primitive* clone() const = 0;

};

class Float : public Primitive {
public:

/* Constructors */
explicit Float(const double& v) : v_(v) {
#ifdef DEBUG_PRIMITIVE
    std::cout << "Float(const double& v) : this = " << (void*) this << " v = " << v << std::endl;
#endif
}

explicit Float(const Float& f) : v_(f.v_) {
#ifdef DEBUG_PRIMITIVE
    std::cout << "Float(const Float& f) : this = " << (void*) this << " v = " << f.v_ << std::endl;
#endif
}

/* Destructor */
~Float() override {
#ifdef DEBUG_PRIMITIVE
    std::cout << "Entering ~Float() : this = " << (void*) this << " v = " << v_ << std::endl;
#endif
    OBJECT_DESCTRUCT(this);
#ifdef DEBUG_PRIMITIVE
    std::cout << "Leaving ~Float() : this = " << (void*) this << " v = " << v_ << std::endl;
#endif
}

/* Assignment */
Float& operator=(const double& v) {
    v_ = v;

    return *this;
}

/* Getter */
double get() const {
    return v_;
}

/* Miscellanous */
bool is_equal(const Object* o) const override; 

string to_string() const override {
    return std::to_string(v_);
}

Primitive* clone() const override {
    return new Float(v_);
}

private:

double v_;

};

class Int : public Primitive {
public:

/* Constructors */
explicit Int(const int64_t& v) : v_(v) {
#ifdef DEBUG_PRIMITIVE
    std::cout << "Int(const int64_t& v) : this = " << (void*) this << " v = " << v << std::endl;
#endif
}

explicit Int(const Int& i) : v_(i.v_) {
#ifdef DEBUG_PRIMITIVE
    std::cout << "Int(const Int& i) : this = " << (void*) this << " v = " << i.v_ << std::endl;
#endif
}

/* Destructor */
~Int() override {
#ifdef DEBUG_PRIMITIVE
    std::cout << "Entering ~Int() : this = " << (void*) this << " v = " << v_ << std::endl;
#endif
    OBJECT_DESCTRUCT(this);
#ifdef DEBUG_PRIMITIVE
    std::cout << "Leaving ~Int() : this = " << (void*) this << " v = " << v_ << std::endl;
#endif
}


/* Assignment */
Int& operator=(const int64_t& v) {
    v_ = v;

    return *this;
}

/* Getter */
int64_t get() const {
    return v_;
}

/* Miscellanous */
bool is_equal(const Object* o) const override {
    if (dynamic_cast<const Int*>(o) != nullptr) {
        return dynamic_cast<const Int*>(o)->v_ == v_;
    } else if (dynamic_cast<const Float*>(o) != nullptr) {
        return dynamic_cast<const Float*>(o)->get() == v_;
    } else {
        return false;
    }
}

Primitive* clone() const override {
    return new Int(v_);
}

string to_string() const override {
    return std::to_string(v_);
}

private:

int64_t v_;
    
};

bool Float::is_equal(const Object* o) const {
    if (dynamic_cast<const Float*>(o) != nullptr) {
        return dynamic_cast<const Float*>(o)->v_ == v_;
    } else if (dynamic_cast<const Int*>(o) != nullptr) {
        return dynamic_cast<const Int*>(o)->get() == v_;
    } else {
        return false;
    }
}

};
