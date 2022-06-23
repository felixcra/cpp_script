#include <catch2/catch_test_macros.hpp>

#include "primitives.hpp"

using namespace cs;

TEST_CASE("test_get") {
    Int i(2);
    REQUIRE(i.get() == 2);

    Float f(3.5);
    REQUIRE(f.get() == 3.5);
}

TEST_CASE("test_equality") {
    Int i(2);
    Float f(2.0);
    REQUIRE(i.get() == 2.0);
    REQUIRE(f.get() == 2);
    REQUIRE(i == 2.0);
    REQUIRE(f == 2);
    REQUIRE(f == i);
}

TEST_CASE("test_int_inequality") {
    Int i(2);
    REQUIRE(i <= 2);
    REQUIRE(i <= Int(2));
    REQUIRE(i <= Float(2.0));
    REQUIRE(i <= 3);
    REQUIRE(i <= Int(3));
    REQUIRE(i <= Float(3.0));
}

TEST_CASE("test_float_inequality") {
    Float f(2.0);
    REQUIRE(f <= 2);
    REQUIRE(f <= Int(2));
    REQUIRE(f <= Float(2.0));
    REQUIRE(f <= 3);
    REQUIRE(f <= Int(3));
    REQUIRE(f <= Float(3.0));
}
