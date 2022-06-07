#include <catch2/catch_test_macros.hpp>

#include <sstream>

#include "list.hpp"
#include "primitive.hpp"

using namespace cs;

TEST_CASE("test_list_1") {
    List l1{1,2,3};
    List l2{1,2,3};
    List l3{0,2,3};
    List l4{Primitive(1),2,3};
    REQUIRE(l1 == l2);
    REQUIRE(l1 != l3);
    REQUIRE(l1 == l4);
}

TEST_CASE("test_list_2") {
    List l1{1,2,3};
    std::stringstream s;
    s << l1;
    REQUIRE(s.str() == "[1,2,3]");
}
