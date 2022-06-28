#include <catch2/catch_test_macros.hpp>

#include <string>
#include <string.h>
#include <type_traits>
#include <vector>

#include "cs.hpp"

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

TEST_CASE("test_element") {
    Element e(2);
    e = 3;
    int a = 4;
    e = a;
    const int b = 4;
    e = b;
}

TEST_CASE("from_vector_1") {
    std::vector<uint> v;
    for (size_t i = 0; i < 10; ++i) v.push_back(i);
    Dict d;
    d["v"] = v;
    REQUIRE(d["v"] == List(Range(10)));
}

TEST_CASE("from_vector_2") {
    std::vector<std::vector<uint>> v;
    for (size_t i = 0; i < 10; ++i) {
        v.push_back(std::vector<uint>());
        for (size_t j = 0; j < 10; ++j) {
            v.back().push_back(j+10*i);
        }
    }
    Dict d;
    d["v"] = v;
    REQUIRE(d["v"] == List(Range(10),[](auto i){
        return List(Range(10),[&](auto j){
            return j+10*i;
        });
    }));
}

TEST_CASE("test_hold_bool_1") {
    Element et(true);
    Element ef(false);
    REQUIRE(et == true);
    REQUIRE(ef == false);
    REQUIRE(et.as<Bool>().get() == true);
    REQUIRE(ef.as<Bool>().get() == false);
    Element ex(10);
    ex = et;
    REQUIRE(ex == true);
    ex = ef;
    REQUIRE(ex == false);
}

TEST_CASE("test_hold_bool_2") {
    Element et(true);
    Element ef(false);
    REQUIRE(et.to_string() == "true");
    REQUIRE(ef.to_string() == "false");
}
