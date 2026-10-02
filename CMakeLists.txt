cmake_minimum_required(VERSION 4.0)
project(bit_array)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(bit_array
        BitArray.cpp
        BitArray.h
        BitArrayTests.cpp)

target_link_libraries(bit_array PRIVATE gtest_main)


include(GoogleTest)
gtest_discover_tests(bit_array)
