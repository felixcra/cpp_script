#include <catch2/catch_test_macros.hpp>
#include <string>

#include "range.hpp"

using namespace cs;

TEST_CASE("test_range_1") {
    REQUIRE(Range(5).to_string() == "Range(0,5)");
    REQUIRE(Range(2,7).to_string() == "Range(2,7)");
}

TEST_CASE("test_range_2") {
    std::string s = "";
    for (unsigned int i : Range(2,5)) s += std::to_string(i);
    REQUIRE(s == "234");
}
