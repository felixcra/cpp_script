#include <catch2/catch_test_macros.hpp>

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
    List l2{4,5,6};
    List l3{l1,l2,1};
    REQUIRE(l1.to_string() == "[1,2,3]");
    REQUIRE(l3.to_string() == "[[1,2,3],[4,5,6],1]");
}
