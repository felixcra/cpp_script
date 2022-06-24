/* 
 * Author: Felix Crazzolara
 */ 

#pragma once

#include "element.hpp"

namespace cs {

template <typename T>
requires std::is_arithmetic_v<T>
Element& Element::operator=(const vector<T>& v) {
    List* l = new List();
    r_.set(*l,true);

    for (const auto& t : v) l->append(t);

    return *this;
}

};
