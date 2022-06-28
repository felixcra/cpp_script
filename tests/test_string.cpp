#include <catch2/catch_test_macros.hpp>

#include <string>

#include "string.hpp"

using namespace cs;

TEST_CASE("test_string") {
    String s("hello");
    REQUIRE(s == std::string("hello"));
    REQUIRE(s != std::string("hellox"));
    REQUIRE(s.to_string() == std::string("hello"));
}

TEST_CASE("test_assignment") {
    String s("hello");
    s = std::string("world");
    REQUIRE(s == std::string("world"));
    s = "foo";
    REQUIRE(s == "foo");
}
