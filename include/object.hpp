/* 
 * Author: Felix Crazzolara
 */ 

#pragma once

#include <type_traits>
#include <memory>
#include <vector>
#include <algorithm>
#include <cassert>
#include <string>

namespace cs {

/* Type definitions */
template <typename T>
using unique_ptr = std::unique_ptr<T>;
template <typename T>
using vector     = std::vector<T>;
using string     = std::string;

class ObjectRef;

class Object {
public:

Object() : registry_(new Registry()) {}

explicit Object(Object& o) = delete;

explicit Object(const Object& o) = delete;

explicit Object(const Object&& o) = delete;

explicit Object(Object&& o) = delete;

virtual ~Object();

virtual bool is_equal(const Object* o) const {
    return this == o;
}

virtual string to_string() const {
    return "Object()";
}

private:

struct Registry {

void add(ObjectRef* r) {
    refs_.push_back(r);
}

vector<ObjectRef*> refs_;

};

unique_ptr<Registry> registry_;

friend class ObjectRef;
friend class ObjectFriend;
friend class Element;

void add_reference(ObjectRef* r) const {
    registry_->add(r);
}

};

class ObjectRef {
public:

ObjectRef() : o_(nullptr), v_(false), d_(false) {}

explicit ObjectRef(ObjectRef& r) : ObjectRef(static_cast<const ObjectRef&>(r)) {}

explicit ObjectRef(const ObjectRef& r) {
    this->o_ = r.o_;
    this->v_ = r.v_;
    this->d_ = r.d_;
    o_->add_reference(this);
}

explicit ObjectRef(ObjectRef&& r) : ObjectRef(r) {}

explicit ObjectRef(const ObjectRef&& r) : ObjectRef(r) {}

ObjectRef& operator=(const ObjectRef& r) {
    this->o_ = r.o_;
    this->v_ = r.v_;
    this->d_ = r.d_;
    o_->add_reference(this);

    return *this;
}

private:

void remove_from_registry() const {
    // Remove this reference from the object registry
    auto& refs    = o_->registry_->refs_;
    const auto it = std::find(refs.begin(), refs.end(), this);
    assert(it != refs.end());
    refs.erase(it);
    
    // Delete the object if there are no more references to it and the object reference was created
    // from an r-value reference or copied
    if (d_ && refs.size() == 0) {
        delete o_;
    }
}

public:

~ObjectRef() {
    if (v_) {
        remove_from_registry();
    }
}

template <typename T>
void set(const T& v, const bool d) {
    static_assert(std::is_base_of<Object,T>::value);

    if (v_) {
        remove_from_registry();
    }

    o_ = &v;
    v_ = true;
    d_ = d;
    v.add_reference(this);
}

const Object* get() const {
    return o_;
}

private:

friend class Object;
friend class ObjectFriend;
friend class Element;

const Object* o_;

bool v_;
bool d_;

};

Object::~Object() {
    // registry_ might be a nullptr if the current object was moved or copied by OBJECT_DESCTRUCT
    if (registry_.get() != nullptr) {
        for (ObjectRef* ref : registry_->refs_) {
            ref->v_ = false;
        }
    }
}

class ObjectFriend {

public:

template <typename T>
static void OBJECT_DESCTRUCT(T* o) {
    if (o->registry_->refs_.size() > 0) {
        // Create a copy of the object
        T* n = new T(*o);

        // Move the registry
        n->registry_ = std::move(o->registry_);

        // Update all references
        for (ObjectRef* ref : n->registry_->refs_) {
            ref->o_ = n;
            assert(ref->v_);
            ref->d_ = true;
        }
    }
}

};

template <typename T>
void OBJECT_DESCTRUCT(T* o) {
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);

    ObjectFriend::OBJECT_DESCTRUCT(o);
}

};
