/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#include <string>

#ifdef DEBUG_PRIMITIVE
#include <iostream>
#endif

#include "object.hpp"

namespace cs {

/* Type definitions */
using string = std::string;

class String : public virtual Object {
public:

/* Constructors */
explicit String(const string& s) : s_(s) {
#ifdef DEBUG_STRING
    std::cout << "String(const string& s) : this = " << (void*) this << " s = " << s << std::endl;
#endif
}

explicit String(string&& s) : s_(std::move(s)) {
#ifdef DEBUG_STRING
    std::cout << "String(string&& s) : this = " << (void*) this << " s = " << s << std::endl;
#endif
}

explicit String(const String& s) : s_(s.s_) {
#ifdef DEBUG_STRING
    std::cout << "String(const String& s) : this = " << (void*) this << " s = " << s.s_ << std::endl;
#endif
}

/* Destructor */
~String() override {
#ifdef DEBUG_STRING
    std::cout << "~String() : this = " << (void*) this << " s = " << s_ << std::endl;
#endif
    OBJECT_DESCTRUCT(this);
}

/* Boolean operators */
bool operator==(const string& s) const {
    return s_ == s;
}

bool operator!=(const string& s) const {
    return !(*this == s);
}

/* Getter */
string get() const {
    return s_;
}

/* Miscellanous */
bool is_equal(const Object* o) const override {
    return dynamic_cast<const String*>(o) != nullptr &&
           dynamic_cast<const String*>(o)->s_ == s_;
}

string to_string() const override {
    return s_;
}

private:

string s_;

};

};
