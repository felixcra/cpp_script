#include <catch2/catch_test_macros.hpp>

#include "tuple.hpp"
#include "primitives.hpp"

using namespace cs;

TEST_CASE("test_tuple_1") {
    Tuple t1(1,2,3);
    Tuple t2(1,2,3);
    Tuple t3(0,2,3);
    Tuple t4(Int(1),2,3);
    REQUIRE(t1 == t2);
    REQUIRE(t1 != t3);
    REQUIRE(t1 == t4);
}

TEST_CASE("test_tuple_2") {
    Tuple t0(0);
    Tuple t1(1,2,3);
    Tuple t2(4,5,6);
    Tuple t3(t1,t2,1);
    REQUIRE(t0.to_string() == "(0,)");
    REQUIRE(t1.to_string() == "(1,2,3)");
    REQUIRE(t3.to_string() == "((1,2,3),(4,5,6),1)");
}

TEST_CASE("test_tuple_3") {
    int i = 4;
    Tuple t(i);
    REQUIRE(t[0] == 4);
    for (auto e : t) ;
    for (Element& e : t) ;
    for (const Element& e : t) ;
    for (Element e : t) ;
}
