#include <catch2/catch_test_macros.hpp>

#include "dict.hpp"
#include "primitive.hpp"

using namespace cs;

TEST_CASE("test_simple_access_1") {
    Dict d;
    d[Primitive(2)] = Primitive(3);
    REQUIRE(d[Primitive(2)] == 3);
    REQUIRE(d[2] == 3);
    REQUIRE(d[2] == Primitive(3));
}

TEST_CASE("test_simple_access_2") {
    Dict d;
    d["1"] = 1;
    REQUIRE(d["1"] == 1);
    REQUIRE(static_cast<const Dict>(d)["1"] == 1);
    std::string s = "1";
    REQUIRE(d[s] == 1);
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
