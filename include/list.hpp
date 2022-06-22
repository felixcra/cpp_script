/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#include <utility>
#include <ostream>
#include <string>

#ifdef DEBUG_LIST
#include <iostream>
#endif

#include "object.hpp"
#include "element.hpp"
#include "container.hpp"
#include "range.hpp"

namespace cs {

/* Type definitions */
using string = std::string;

template <typename T>
concept derived_from_Iterable = requires(T& x) {
    []<typename U>(Iterable<U>&){}(x);
};

class List : public Container, public virtual Object {

friend class ListFriend;

public:

/* Constructors */
List() {
#ifdef DEBUG_LIST
    std::cout << "List() : this = " << (void*) this << std::endl;
#endif
}

template <typename... Types>
requires ((std::tuple_size_v<std::tuple<Types...>> != 1) ||
         (!derived_from_Iterable<std::remove_cvref_t<std::tuple_element_t<0,std::tuple<Types...>>>>))
explicit List(Types&&... args) {
#ifdef DEBUG_LIST
    std::cout << "List(Types&&... args) : this = " << (void*) this << std::endl;
#endif
    (elems_.emplace_back(std::forward<Types>(args)),...);
}

explicit List(const List& l) {
#ifdef DEBUG_LIST
    std::cout << "List(const List& l) : this = " << (void*) this << std::endl;
#endif
    for (const Element& e : l.elems_) elems_.emplace_back(e,true);
}

template <typename C>
requires std::is_base_of_v<Container,C>
explicit List(const C& c) {
#ifdef DEBUG_LIST
    std::cout << "List(const C& c) : this = " << (void*) this << std::endl;
#endif
    for (const Element& e : c) elems_.emplace_back(e,true);
}

explicit List(const Range& r) {
#ifdef DEBUG_LIST
    std::cout << "List(const Range& r) : this = " << (void*) this << std::endl;
#endif
    for (const uint& v : r) elems_.emplace_back(v);
}

/* Destructor */
~List() override {
    OBJECT_DESCTRUCT(this);
}

/* Assignment */
List& operator=(const List& l) {
    for (const Element& e : l.elems_) elems_.emplace_back(e);
    return *this;
}

/* Modification methods */
template <typename T>
void append(T&& e) {
    elems_.emplace_back(std::forward<T>(e));
}

/* Accessor methods */
const Element& operator[](const int& i) const {
    return elems_.at(i);
}

Element& operator[](const int& i) {
    return elems_.at(i);
}

/* Miscellaneous */
bool is_equal(const Object* o) const override {
    // Check if o is a list
    if (dynamic_cast<const List*>(o) == nullptr) return false;
    const List& l = *dynamic_cast<const List*>(o);

    // Check that o has the correct number of elements
    if (elems_.size() != l.elems_.size()) return false;
    if (elems_.size() == 0 && l.elems_.size() == 0) return true;

    for (size_t i = 0; i < elems_.size(); ++i) {
        if (elems_.at(i) != l.elems_.at(i)) {
            return false;
        }
    }

    return true;
}

string to_string() const override {
    string s = "[";
    for (size_t i = 0; i < elems_.size(); ++i) {
        s += elems_[i].to_string();
        if (i != elems_.size()-1) s += ",";
    }
    s += "]";
    return s;
}

friend std::ostream& operator<<(std::ostream& out, const List& l);

};

std::ostream& operator<<(std::ostream& out, const List& l) {
    return out << l.to_string();
}

class ListFriend {

public:

static int length(const List& l) {
    return l.elems_.size();
}

};

int len(const List& l) {
    return ListFriend::length(l);
}

};
