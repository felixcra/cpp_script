/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#include <initializer_list>
#include <vector>

#include <ostream>

#include "element.hpp"

namespace cs {

/* Type definitions */
template <typename T>
using initializer_list = std::initializer_list<T>;
template <typename T>
using vector           = std::vector<T>;

class List {

friend class ListFriend;

vector<Element> elems_;

public:

List() {}

template <typename... T>
explicit List(T&&... args) {
    (elems_.emplace_back(std::move(args)),...);
}

template <typename T>
void append(T& v) {
    append(static_cast<const T&>(v));
}

template <typename T>
void append(const T& v) {
    elems_.emplace_back(v);
}

template <typename T>
void append(T&& e) {
    elems_.emplace_back(std::move(e));
}

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

const Element& operator[](const int& i) const {
    return elems_.at(i);
}

Element& operator[](const int& i) {
    return elems_.at(i);
}

friend std::ostream& operator<<(std::ostream& out, const List& l);

};

std::ostream& operator<<(std::ostream& out, const List& l) {
    out << "[";
    for (size_t i = 0; i < l.elems_.size(); ++i) {
        out << l.elems_[i];
        if (i != l.elems_.size()-1) out << ",";
    }
    out << "]";
    return out;
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
