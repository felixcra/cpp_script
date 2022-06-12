/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#include <vector>
#include <utility>
#include <ostream>

#ifdef DEBUG
#include <iostream>
#endif

#include "object.hpp"
#include "element.hpp"

namespace cs {

/* Type definitions */
template <typename T>
using vector = std::vector<T>;

class List : public virtual Object {

friend class ListFriend;

vector<Element> elems_;

public:

/* Constructors */
List() {}

template <typename... Types>
explicit List(Types&&... args) {
#ifdef DEBUG
    std::cout << "List(Types&&... args) : this = " << (void*) this << std::endl;
#endif
    (elems_.emplace_back(std::forward<Types>(args)),...);
}

List(const List& l) {
#ifdef DEBUG
    std::cout << "List(const List& l) : this = " << (void*) this << std::endl;
#endif
    for (const Element& e : l.elems_) elems_.emplace_back(e);
}

/* Destructor */
~List() override {
    OBJECT_DESCTRUCT(this);
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

/* Boolean operators */
bool operator==(const List& l) const {
    if (elems_.size() != l.elems_.size()) return false;
    if (elems_.size() == 0 && l.elems_.size() == 0) return true;

    for (size_t i = 0; i < elems_.size(); ++i) {
        if (elems_.at(i) != l.elems_.at(i)) {
            return false;
        }
    }

    return true;
}

bool operator!=(const List& l) const {
    return !(*this == l);
}

/* Miscellaneous */
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
