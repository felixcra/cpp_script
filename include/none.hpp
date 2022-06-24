/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#ifdef DEBUG_NONE
#include <iostream>
#endif

#include "object.hpp"

namespace cs {

class None : public virtual Object {
public:

/* Constructors */
None() {
#ifdef DEBUG_NONE
    std::cout << "None() : this = " << (void*) this << std::endl;
#endif
}

None(const None& n) {
#ifdef DEBUG_NONE
    std::cout << "None(const None& n) : this = " << (void*) this << std::endl;
#endif
}

/* Destructor */
~None() override {
#ifdef DEBUG_NONE
    std::cout << "Entering ~None() : this = " << (void*) this << std::endl;
#endif
    OBJECT_DESCTRUCT(this);
#ifdef DEBUG_NONE
    std::cout << "Leaving ~None() : this = " << (void*) this << std::endl;
#endif
}

/* Miscellanous */
string to_string() const override {
    return "None";
}

};

};
