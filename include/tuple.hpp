/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

#include "object.hpp"
#include "element.hpp"
#include "container.hpp"

namespace cs {

/* Type definitions */
using string = std::string;

class Tuple : public Container, public virtual Object {

public:

/* Constructors */
template <typename... Types>
explicit Tuple(Types&&... args) {
#ifdef DEBUG_TUPLE
    std::cout << "Tuple(Types&&... args) : this = " << (void*) this << std::endl;
#endif
    (elems_.emplace_back(std::forward<Types>(args)),...);
}

Tuple(const Tuple& t) {
#ifdef DEBUG_TUPLE
    std::cout << "Tuple(const Tuple& t) : this = " << (void*) this << std::endl;
#endif
    for (const Element& e : t.elems_) elems_.emplace_back(e);
}

/* Destructor */
~Tuple() override {
    OBJECT_DESCTRUCT(this);
}

/* Accessor methods */
const Element& operator[](const int& i) const {
    return elems_.at(i);
}

/* Miscellaneous */
bool is_equal(const Object* o) const override {
    // Check if o is a tuple
    if (dynamic_cast<const Tuple*>(o) == nullptr) return false;
    const Tuple& t = *dynamic_cast<const Tuple*>(o);

    // Check that o has the correct number of elements
    if (elems_.size() != t.elems_.size()) return false;

    for (size_t i = 0; i < elems_.size(); ++i) {
        if (elems_.at(i) != t.elems_.at(i)) {
            return false;
        }
    }

    return true;
}

string to_string() const override {
    string s = "(";
    for (size_t i = 0; i < elems_.size(); ++i) {
        s += elems_[i].to_string();
        if (elems_.size() == 1)        s += ",";
        else if (i != elems_.size()-1) s += ",";
    }
    s += ")";
    return s;
}

};

};
