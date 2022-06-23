#include <catch2/catch_test_macros.hpp>

#include "dict.hpp"
#include "primitives.hpp"
#include "list.hpp"

using namespace cs;

TEST_CASE("test_simple_access_1") {
    Dict d;
    d[Int(2)] = Int(3);
    REQUIRE(d[Int(2)] == 3);
    REQUIRE(d[2] == 3);
    REQUIRE(d[2] == Int(3));
}

TEST_CASE("test_simple_access_2") {
    Dict d;
    d["1"] = 1;
    REQUIRE(d["1"] == 1);
    REQUIRE(static_cast<const Dict>(d)["1"] == 1);
    std::string s = "1";
    REQUIRE(d[s] == 1);
}

TEST_CASE("test_simple_access_3") {
    Dict d;
    const unsigned int i = 0;
    d["P"] = i;
    bool throws_exception = false;
    try {
        d["1"].to_string();
    } catch (const Dict::KeyError& e) {
        throws_exception = true; 
    }
    REQUIRE(throws_exception == true);
    REQUIRE(d["P"] == 0u);
    REQUIRE(d["P"] == 0);
    REQUIRE(static_cast<const Dict>(d)["P"] == 0);
    std::string s = "P";
    REQUIRE(d[s] == 0);
}

class C : public Object {
public:

/* Constructors */
C(const C& c) {}

/* Destructor */
~C() override {
    OBJECT_DESCTRUCT(this);
}

//size_t hash() override {
//    return 0;
//}

};

TEST_CASE("test_collision") {
    Dict d;
    d[Object()] = 2;
    d[Object()] = 3;
    Object o1;
    d[o1] = 4;
}

TEST_CASE("test_resize") {
    Dict d;
    for (size_t i = 0; i < 100; ++i) {
        d[i] = i+2;
    }
    for (size_t i = 0; i < 100; ++i) {
        REQUIRE(d[i] == i+2);
    }
}

TEST_CASE("test_insert_list_1") {
    List l(1,2,3);
    Dict d;
    d["l"] = l;
}

Dict get_dict() {
    Dict d;
    List l(8);
    d["l"] = l;
    return d;
}

TEST_CASE("test_insert_list_2") {
    const Dict d = get_dict();
}
