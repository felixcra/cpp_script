cmake_minimum_required(VERSION 3.20.0)
project(yglu)

set(CMAKE_CXX_STANDARD 17)

include(CTest)

find_package(Catch2 3 REQUIRED)

function(add_tests)
    MATH(EXPR out "${ARGC}-1")
    foreach(i RANGE ${out})
        add_executable(${ARGV${i}}
            tests/${ARGV${i}}.cpp
        )
        target_link_libraries(${ARGV${i}}
            PRIVATE cs
            PRIVATE Catch2::Catch2WithMain
        )
        add_test(
            NAME ${ARGV${i}}
            COMMAND ${ARGV${i}}
        )
    endforeach()
endfunction()

add_library(cs INTERFACE)
target_include_directories(cs
    INTERFACE include
)

add_tests(
    test_object_and_element
    test_list
)
