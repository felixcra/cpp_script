/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#include "dict.hpp"

namespace cs {

class OrderedDict : public Dict {
public:

/* Destructor */
~OrderedDict() override {
    OBJECT_DESCTRUCT(this);
}

};

};
