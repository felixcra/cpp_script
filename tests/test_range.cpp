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
    for (int i : Range(2,5)) s += std::to_string(i);
    REQUIRE(s == "234");
}

TEST_CASE("test_range_3") {
    std::string s = "";
    for (int i : Range(-5)) s += std::to_string(i);
    REQUIRE(s == "");
}

TEST_CASE("test_range_4") {
    std::string s = "";
    for (int i : Range(Int(4))) s += std::to_string(i);
    REQUIRE(s == "0123");
}
