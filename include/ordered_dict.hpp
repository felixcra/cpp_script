/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#ifdef DEBUG_ORDERED_DICT
#include <iostream>
#endif

#include "dict.hpp"

namespace cs {

class OrderedDict : public Dict {
public:

/* Destructor */
~OrderedDict() override {
#ifdef DEBUG_ORDERED_DICT
    std::cout << "~OrderedDict() : this = " << (void*) this << std::endl;
#endif
    OBJECT_DESCTRUCT(this);
}

};

};
