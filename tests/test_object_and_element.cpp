#include <catch2/catch_test_macros.hpp>

#include <string>
#include <string.h>
#include <type_traits>

#include "object.hpp"
#include "list.hpp"
#include "element.hpp"

using namespace cs;

class NamedObject : public Object {
public:

NamedObject(const std::string&& name) : name(name) {}

NamedObject(const NamedObject& no) : name(no.name) {}

~NamedObject() override {
    OBJECT_DESCTRUCT(this);
}

const std::string name;

};

TEST_CASE("test_object") {
    Object o1;
    Object o2;
    REQUIRE(o1 == o1);
    REQUIRE(o1 != o2);
}

TEST_CASE("test_Element_l_value_construction") {
    class C : public Object {
        public:
            C() {}

            C(const C& c) {}

            ~C() override {
                OBJECT_DESCTRUCT(this);
            }

            bool is_equal(const Object* o) const override {
                return o == this;
            }
    };
    C c;
    Element e(c);
}

TEST_CASE("test_Element_r_value_construction") {
    class C : public Object {
        public:
            C() {}

            C(const C& c) {}

            ~C() override {
                OBJECT_DESCTRUCT(this);
            }

            bool is_equal(const Object* o) const override {
                return this == o;
            }
    };
    Element e{C()};
}

TEST_CASE("test_register") {
    static_assert(std::is_copy_assignable_v<Element>);
    static_assert(std::is_copy_constructible_v<Element>);

    List l;
    l.append(Object());
    {
        NamedObject named_obj("hello");
        l.append(named_obj);
    }
    REQUIRE(len(l) == 2);
    l[0].as<Object>();
    l[1].as<NamedObject>();
    REQUIRE(l[1].as<NamedObject>().name == "hello");
}

TEST_CASE("test_list_modification") {
    List l;
    l.append(Object());
    l[0] = NamedObject("hello");
    REQUIRE(l[0].as<NamedObject>().name == "hello");
}
