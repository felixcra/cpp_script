#include <catch2/catch_test_macros.hpp>

#include "list.hpp"
#include "primitive.hpp"
#include "element.hpp"
#include "tuple.hpp"

using namespace cs;

class Base : public virtual Object {
public:

/* Constructors */
Base() {}

explicit Base(const Base& b) {}

/* Destructor */
~Base() override {
    OBJECT_DESCTRUCT(this);
}

};

class Derived : public Base {
public:

/* Constructors */
Derived() {}

explicit Derived(const Derived& d) : Base(d) {}

/* Destructor */
~Derived() override {
    OBJECT_DESCTRUCT(this);
}

};

TEST_CASE("test_list_1") {
    List l1(1,2,3);
    List l2(1,2,3);
    List l3(0,2,3);
    List l4(Primitive(1),2,3);
    REQUIRE(l1 == l2);
    REQUIRE(l1 != l3);
    REQUIRE(l1 == l4);
}

TEST_CASE("test_list_2") {
    List l0(0);
    List l1(1,2,3);
    List l2(4,5,6);
    List l3(l1,l2,1);
    REQUIRE(l0.to_string() == "[0]");
    REQUIRE(l1.to_string() == "[1,2,3]");
    REQUIRE(l3.to_string() == "[[1,2,3],[4,5,6],1]");
}

TEST_CASE("test_list_3") {
    List l1(1,2,3,Object());
    List l2;
    l2    = l1;
    l1[1] = 5;
    l1[3] = 4;
    REQUIRE(l2[1] == 5);
    REQUIRE(l2[2] == 3);
    REQUIRE(l1[3] == 4);
}

TEST_CASE("test_list_4") {
    int i = 4;
    List l(i);
    REQUIRE(l[0] == 4);
}

TEST_CASE("test_list_5") {
    List l1, l2;
    l1.append(std::string("hello"));
    REQUIRE(l1[0] == "hello");
    REQUIRE(l1[0] != "hellox");
    REQUIRE(l1[0] == string("hello"));
    REQUIRE(l1[0] != string("hellox"));
    l2.append("hello");
    REQUIRE(l1[0] == l2[0]);
}

TEST_CASE("test_list_6") {
    List l;
    Derived d;
    l.append(d);
}

TEST_CASE("test_from_tuple") {
    Tuple t1(1,2,3);
    List l1(t1);
    REQUIRE(l1.to_string() == "[1,2,3]");
}

TEST_CASE("test_from_list_1") {
    List l1(1,2,3);
    List l2;
    l2 = l1;
    REQUIRE(l1.to_string() == "[1,2,3]");
    REQUIRE(l2.to_string() == "[1,2,3]");
    l1[1] = 5;
    REQUIRE(l1.to_string() == "[1,5,3]");
    REQUIRE(l2.to_string() == "[1,5,3]");
}

TEST_CASE("test_from_list_2") {
    List l1(1,2,3);
    List l2(l1);
    l1[1] = 5;
    REQUIRE(l1.to_string() == "[1,5,3]");
    REQUIRE(l2.to_string() == "[1,2,3]");
}

TEST_CASE("test_from_range") {
    REQUIRE(List(Range(2,5)).to_string() == "[2,3,4]");
    REQUIRE(List(Range(2,5),[](auto i){return 1;}).to_string() == "[1,1,1]");
}
