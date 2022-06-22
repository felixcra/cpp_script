cmake_minimum_required(VERSION 3.20.0)
project(yglu)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_OBJECT")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_PRIMITIVE")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_ELEMENT")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_DICT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_LIST")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_ITERABLE")

set(BUILD_TEST FALSE)

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
        add_test(${ARGV${i}} ${ARGV${i}})
    endforeach()
endfunction()

add_library(cs INTERFACE)
target_include_directories(cs
    INTERFACE include
)

if (BUILD_TESTS)
    add_tests(
        test_object_and_element
        test_primitive
        test_list
        test_dict
        test_string
        test_tuple
        test_range
    )
endif()
