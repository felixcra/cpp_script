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
    REQUIRE(f == i);
}
