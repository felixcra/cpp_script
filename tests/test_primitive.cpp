#include <catch2/catch_test_macros.hpp>

#include "primitive.hpp"

using namespace cs;

TEST_CASE("test_get") {
    Primitive<int> p(2);
    REQUIRE(p.get() == 2);
}
