/* 
 * Author: Felix Crazzolara
 */ 

#pragma once

#include "cs.hpp"

namespace cs {

template <is_nested_vector_or_arithmetic T>
Element& Element::operator=(const vector<T>& v) {
    List* l = new List();
    r_.set(*l,true);

    if constexpr (std::is_arithmetic_v<T>) {
        for (const auto& t : v) l->append(t);
    } else {
        for (const auto& t : v) {
            l->append(None());
            (*l)[-1] = t;
        }
    }


    return *this;
}

};
