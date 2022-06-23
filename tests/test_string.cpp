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
